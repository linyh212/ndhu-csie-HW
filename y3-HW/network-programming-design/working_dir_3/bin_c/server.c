#include "shell.h"
#include <sys/select.h>
#include <sys/un.h>
#include <signal.h>
#include <mysql/mysql.h>

User users[MAX_USER];
int user_count = 0;
pthread_mutex_t user_mutex = PTHREAD_MUTEX_INITIALIZER;
int listen_fd, local_fd;
int client_fd_to_user_index[MAX_USER];
int msg_fd_to_user_index[MAX_USER];

/* ================ HW3 ================ */
MYSQL *db_conn;
pthread_mutex_t db_mutex = PTHREAD_MUTEX_INITIALIZER;

void init_db(void)
{
    db_conn = mysql_init(NULL);
    if (!mysql_real_connect(db_conn, "localhost", "root", "password", "chatdb", 0, NULL, 0))
    {
        fprintf(stderr, "MySQL connect error: %s\n", mysql_error(db_conn));
        exit(1);
    }
    printf("MySQL connected successfully!\n");
}

static void trim_crlf(char *str)
{
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r'))
        str[--len] = '\0';
}

int user_exists(const char *username)
{
    char uname[256];
    strncpy(uname, username, sizeof(uname) - 1);
    uname[sizeof(uname) - 1] = '\0';
    trim_crlf(uname);
    char q[256];
    snprintf(q, sizeof(q), "SELECT 1 FROM users WHERE username='%s'", uname);
    pthread_mutex_lock(&db_mutex);
    if (mysql_query(db_conn, q))
    {
        pthread_mutex_unlock(&db_mutex);
        return 0;
    }
    MYSQL_RES *res = mysql_store_result(db_conn);
    int exists = (res && mysql_num_rows(res) > 0);
    if (res)
        mysql_free_result(res);
    pthread_mutex_unlock(&db_mutex);
    return exists;
}

int verify_password(const char *username, const char *password)
{
    char uname[256];
    strncpy(uname, username, sizeof(uname) - 1);
    uname[sizeof(uname) - 1] = '\0';
    trim_crlf(uname);
    char q[256];
    snprintf(q, sizeof(q), "SELECT password FROM users WHERE username='%s'", uname);
    pthread_mutex_lock(&db_mutex);
    if (mysql_query(db_conn, q))
    {
        pthread_mutex_unlock(&db_mutex);
        return 0;
    }
    MYSQL_RES *res = mysql_store_result(db_conn);
    if (!res || mysql_num_rows(res) == 0)
    {
        if (res)
            mysql_free_result(res);
        pthread_mutex_unlock(&db_mutex);
        return 0;
    }
    MYSQL_ROW row = mysql_fetch_row(res);
    int ok = (row && strcmp(row[0], password) == 0);
    mysql_free_result(res);
    pthread_mutex_unlock(&db_mutex);
    return ok;
}

int register_user(const char *username, const char *password)
{
    if (user_exists(username))
        return 0;
    char uname[256];
    strncpy(uname, username, sizeof(uname) - 1);
    uname[sizeof(uname) - 1] = '\0';
    trim_crlf(uname);
    char pwd[256];
    strncpy(pwd, password, sizeof(pwd) - 1);
    pwd[sizeof(pwd) - 1] = '\0';
    trim_crlf(pwd);
    char q[512];
    snprintf(q, sizeof(q), "INSERT INTO users (username, password) VALUES ('%s', '%s')", uname, pwd);
    pthread_mutex_lock(&db_mutex);
    int ret = mysql_query(db_conn, q);
    pthread_mutex_unlock(&db_mutex);
    return (ret == 0);
}

int add_mail(const char *to, const char *from, const char *content)
{
    char q[1024];
    char *escaped_content = malloc(strlen(content) * 2 + 1);
    mysql_real_escape_string(db_conn, escaped_content, content, strlen(content));
    snprintf(q, sizeof(q), "INSERT INTO mails (to_user, from_user, content) VALUES ('%s', '%s', '%s')", to, from, escaped_content);
    free(escaped_content);
    pthread_mutex_lock(&db_mutex);
    int ret = mysql_query(db_conn, q);
    pthread_mutex_unlock(&db_mutex);
    return (ret == 0);
}

char *list_mails(const char *username)
{
    char q[256];
    snprintf(q, sizeof(q), "SELECT id, from_user, content, sent_time FROM mails WHERE to_user='%s' ORDER BY sent_time", username);
    pthread_mutex_lock(&db_mutex);
    if (mysql_query(db_conn, q))
    {
        pthread_mutex_unlock(&db_mutex);
        return strdup("empty !\n");
    }
    MYSQL_RES *res = mysql_store_result(db_conn);
    if (!res || mysql_num_rows(res) == 0)
    {
        if (res)
            mysql_free_result(res);
        pthread_mutex_unlock(&db_mutex);
        return strdup("empty !\n");
    }
    char *result = malloc(4096);
    result[0] = '\0';
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)))
    {
        char line[256];
        snprintf(line, sizeof(line), "ID:%s From:%s Content:%s Time:%s\n", row[0], row[1], row[2], row[3]);
        strcat(result, line);
    }
    mysql_free_result(res);
    pthread_mutex_unlock(&db_mutex);
    return result;
}

int delete_mail(int mail_id, const char *username)
{
    char q[256];
    snprintf(q, sizeof(q), "DELETE FROM mails WHERE id=%d AND to_user='%s'", mail_id, username);
    pthread_mutex_lock(&db_mutex);
    int ret = mysql_query(db_conn, q);
    int affected = mysql_affected_rows(db_conn);
    pthread_mutex_unlock(&db_mutex);
    return (ret == 0 && affected > 0);
}

/* ================ User Management ================ */
int allocate_id(void)
{
    int used[MAX_USER] = {0};
    pthread_mutex_lock(&user_mutex);
    for (int i = 0; i < user_count; i++)
    {
        if (users[i].tcp_fd != -1)
            used[users[i].id] = 1;
    }
    pthread_mutex_unlock(&user_mutex);
    for (int i = 1; i < MAX_USER; i++)
    {
        if (!used[i])
            return i;
    }
    return -1;
}

User *find_user_by_id(int id)
{
    pthread_mutex_lock(&user_mutex);
    for (int i = 0; i < user_count; i++)
    {
        if (users[i].id == id && users[i].tcp_fd != -1)
        {
            pthread_mutex_unlock(&user_mutex);
            return &users[i];
        }
    }
    pthread_mutex_unlock(&user_mutex);
    return NULL;
}

User *find_user_by_name(const char *name)
{
    pthread_mutex_lock(&user_mutex);
    for (int i = 0; i < user_count; i++)
    {
        if (users[i].tcp_fd != -1 && strcmp(users[i].name, name) == 0)
        {
            pthread_mutex_unlock(&user_mutex);
            return &users[i];
        }
    }
    pthread_mutex_unlock(&user_mutex);
    return NULL;
}

void add_user(int tcp_fd, int msg_fd, int id, const char *name)
{
    pthread_mutex_lock(&user_mutex);
    if (user_count >= MAX_USER)
    {
        pthread_mutex_unlock(&user_mutex);
        fprintf(stderr, "Maximum users reached\n");
        return;
    }
    int idx = user_count;
    users[idx].id = id;
    strncpy(users[idx].name, name, MAX_NAME - 1);
    users[idx].name[MAX_NAME - 1] = '\0';
    users[idx].tcp_fd = tcp_fd;
    users[idx].msg_fd = msg_fd;
    client_fd_to_user_index[tcp_fd] = idx;
    msg_fd_to_user_index[msg_fd] = idx;
    user_count++;
    pthread_mutex_unlock(&user_mutex);
}

void update_user_name(int msg_fd, const char *new_name)
{
    pthread_mutex_lock(&user_mutex);
    int idx = msg_fd_to_user_index[msg_fd];
    if (idx >= 0 && idx < user_count && users[idx].msg_fd == msg_fd)
    {
        strncpy(users[idx].name, new_name, MAX_NAME - 1);
        users[idx].name[MAX_NAME - 1] = '\0';
    }
    pthread_mutex_unlock(&user_mutex);
}

void remove_user(int tcp_fd)
{
    pthread_mutex_lock(&user_mutex);
    for (int i = 0; i < user_count; i++)
    {
        if (users[i].tcp_fd == tcp_fd)
        {
            if (users[i].msg_fd != -1)
            {
                msg_fd_to_user_index[users[i].msg_fd] = -1;
                close(users[i].msg_fd);
            }
            if (users[i].tcp_fd != -1)
            {
                client_fd_to_user_index[users[i].tcp_fd] = -1;
                close(users[i].tcp_fd);
            }
            if (i < user_count - 1)
                users[i] = users[user_count - 1];
            user_count--;
            break;
        }
    }
    pthread_mutex_unlock(&user_mutex);
}

void send_to_user(int target_id, const char *msg)
{
    User *u = find_user_by_id(target_id);
    if (u && u->msg_fd != -1)
    {
        write(u->msg_fd, msg, strlen(msg));
        write(u->msg_fd, "\n", 1);
    }
}

void yell_to_all(const char *msg, int sender_id)
{
    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "<%d yelled>: %s", sender_id, msg);
    pthread_mutex_lock(&user_mutex);
    for (int i = 0; i < user_count; i++)
    {
        if (users[i].tcp_fd != -1 && users[i].id != sender_id)
        {
            write(users[i].msg_fd, buffer, strlen(buffer));
            write(users[i].msg_fd, "\n", 1);
        }
    }
    pthread_mutex_unlock(&user_mutex);
}

void handle_local_request(int client_fd)
{
    Request req;
    int n = read(client_fd, &req, sizeof(req));
    if (n <= 0)
    {
        close(client_fd);
        return;
    }
    char response[BUFFER_SIZE] = {0};

    switch (req.cmd)
    {
    case CMD_LOGIN:
    {
        if (!user_exists(req.arg1))
            snprintf(response, BUFFER_SIZE, "USER_NOT_FOUND");
        else if (!verify_password(req.arg1, req.arg2)) snprintf(response, BUFFER_SIZE, "PASSWORD_ERROR");
        else
        {
            update_user_name(req.sender_id, req.arg1);
            snprintf(response, BUFFER_SIZE, "OK");
        }
        break;
    }
    case CMD_REGISTER:
    {
        if (register_user(req.arg1, req.arg2)) snprintf(response, BUFFER_SIZE, "OK");
        else snprintf(response, BUFFER_SIZE, "Register failed!");
        break;
    }
    case CMD_LISTMAIL:
    {
        char *result = list_mails(req.arg1);
        snprintf(response, BUFFER_SIZE, "%s", result);
        free(result);
        break;
    }
    case CMD_MAILTO:
    {
        if (!user_exists(req.arg1))
            snprintf(response, BUFFER_SIZE, "User not found !\n");
        else if (add_mail(req.arg1, req.arg3, req.arg2))
            snprintf(response, BUFFER_SIZE, "send accept!\n");
        else
            snprintf(response, BUFFER_SIZE, "send fail!\n");
        break;
    }
    case CMD_DELMAIL:
    {
        int mail_id = atoi(req.arg1);
        if (delete_mail(mail_id, req.arg2))
            snprintf(response, BUFFER_SIZE, "delete success !\n");
        else
            snprintf(response, BUFFER_SIZE, "Mail id unexist !\n");
        break;
    }
    case CMD_WHO:
    {
        for (int id = 1; id < MAX_USER; id++)
        {
            User *u = find_user_by_id(id);
            if (u)
            {
                char line[128];
                snprintf(line, sizeof(line), "%d\t%s\n", u->id, u->name);
                strcat(response, line);
            }
        }
        if (strlen(response) == 0)
            strcpy(response, "No user online.\n");
        break;
    }
    case CMD_TELL:
    {
        int target_id = atoi(req.arg1);
        User *target = find_user_by_id(target_id);
        if (!target)
            snprintf(response, BUFFER_SIZE, "User %d not found.\n", target_id);
        else
        {
            char msg[BUFFER_SIZE];
            snprintf(msg, BUFFER_SIZE, "<%d told you>: %s", req.sender_id, req.arg2);
            send_to_user(target_id, msg);
            snprintf(response, BUFFER_SIZE, "send accept!\n");
        }
        break;
    }
    case CMD_YELL:
    {
        yell_to_all(req.arg1, req.sender_id);
        snprintf(response, BUFFER_SIZE, "yell accept!\n");
        break;
    }
    case CMD_NAME:
    {
        char *new_name = req.arg1;
        User *existing = find_user_by_name(new_name);
        if (existing)
            snprintf(response, BUFFER_SIZE, "User new name already exists!\n");
        else
        {
            User *self = find_user_by_id(req.sender_id);
            if (self)
            {
                strncpy(self->name, new_name, MAX_NAME - 1);
                self->name[MAX_NAME - 1] = '\0';
                snprintf(response, BUFFER_SIZE, "name change accept!\n");
            }
            else
                snprintf(response, BUFFER_SIZE, "User not found.\n");
        }
        break;
    }
    case CMD_LOGOUT:
    {
        snprintf(response, BUFFER_SIZE, "OK");
        break;
    }
    default:
        snprintf(response, BUFFER_SIZE, "Unknown command.\n");
    }
    write(client_fd, response, strlen(response));
    close(client_fd);
}

void start_local_server(void)
{
    unlink(SOCKET_PATH);
    local_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
    bind(local_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(local_fd, 5);
}

void sigchld_handler(int sig)
{
    (void)sig;
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
}

int main()
{
    for (int i = 0; i < MAX_USER; i++)
    {
        client_fd_to_user_index[i] = -1;
        msg_fd_to_user_index[i] = -1;
    }
    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, sigchld_handler);
    init_db();
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0)
    {
        perror("socket");
        exit(1);
    }
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
    if (bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind");
        exit(1);
    }
    listen(listen_fd, 10);
    start_local_server();
    fd_set readfds;
    printf("Chat Server started on port %d\n", PORT);
    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(listen_fd, &readfds);
        FD_SET(local_fd, &readfds);
        int max_fd = (listen_fd > local_fd) ? listen_fd : local_fd;
        pthread_mutex_lock(&user_mutex);
        for (int i = 0; i < user_count; i++)
        {
            if (users[i].tcp_fd != -1)
            {
                FD_SET(users[i].tcp_fd, &readfds);
                if (users[i].tcp_fd > max_fd)
                    max_fd = users[i].tcp_fd;
            }
        }
        pthread_mutex_unlock(&user_mutex);
        if (select(max_fd + 1, &readfds, NULL, NULL, NULL) < 0)
        {
            if (errno == EINTR)
                continue;
            break;
        }
        if (FD_ISSET(listen_fd, &readfds))
        {
            struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(client_addr);
            int client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addr_len);
            if (client_fd < 0)
                continue;
            int id = allocate_id();
            char default_name[MAX_NAME];
            snprintf(default_name, sizeof(default_name), "user%d", id);
            int sv[2];
            if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) < 0)
            {
                perror("socketpair");
                close(client_fd);
                continue;
            }
            pid_t pid = fork();
            if (pid == 0)
            {
                close(listen_fd);
                close(local_fd);
                close(sv[1]);
                dup2(client_fd, STDIN_FILENO);
                dup2(client_fd, STDOUT_FILENO);
                dup2(client_fd, STDERR_FILENO);
                close(client_fd);

                char msg_fd_str[16];
                snprintf(msg_fd_str, sizeof(msg_fd_str), "%d", sv[0]);
                setenv("MESSAGE_FD", msg_fd_str, 1);
                setenv("PATH", "bin:.", 1);

                char cwd[1024];
                if (getcwd(cwd, sizeof(cwd)) != NULL)
                {
                    char shell_path[1024];
                    snprintf(shell_path, sizeof(shell_path), "%s/shell", cwd);
                    execl(shell_path, "shell", NULL);
                }
                else
                    execl("./shell", "shell", NULL);
                perror("execl failed");
                exit(1);
            }
            else if (pid > 0)
            {
                close(sv[0]);
                add_user(client_fd, sv[1], id, default_name);
            }
            else
            {
                perror("fork");
                close(client_fd);
                close(sv[0]);
                close(sv[1]);
            }
        }
        if (FD_ISSET(local_fd, &readfds))
        {
            int cmd_fd = accept(local_fd, NULL, NULL);
            if (cmd_fd > 0)
                handle_local_request(cmd_fd);
        }
        int disconnected_fds[MAX_USER] = {0};
        int discon_count = 0;
        pthread_mutex_lock(&user_mutex);
        for (int i = 0; i < user_count; i++)
        {
            if (users[i].tcp_fd != -1)
            {
                char buf[1];
                int ret = recv(users[i].tcp_fd, buf, 1, MSG_PEEK | MSG_DONTWAIT);
                if (ret == 0 || (ret < 0 && errno != EAGAIN && errno != EINTR))
                    disconnected_fds[discon_count++] = users[i].tcp_fd;
            }
        }
        pthread_mutex_unlock(&user_mutex);
        for (int i = 0; i < discon_count; i++)
            remove_user(disconnected_fds[i]);
    }
    close(listen_fd);
    close(local_fd);
    mysql_close(db_conn);
    return 0;
}