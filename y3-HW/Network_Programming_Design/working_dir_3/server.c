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
    char *result = malloc(8192);
    result[0] = '\0';
    MYSQL_ROW row;
    int seq = 1;
    while ((row = mysql_fetch_row(res)))
    {
        char line[512];
        /* Show sequential ID starting from 1 instead of DB id */
        snprintf(line, sizeof(line), "ID:%d From:%s Content:%s Time:%s\n", seq, row[1], row[2], row[3]);
        strcat(result, line);
        seq++;
    }
    mysql_free_result(res);
    pthread_mutex_unlock(&db_mutex);
    return result;
}

/* ================ HW3 Group Functions ================ */
static const char *basename_cmd(char *cmd)
{
    if (cmd == NULL)
        return NULL;
    char *base = strrchr(cmd, '/');
    return base ? base + 1 : cmd;
}

static void trim_whitespace(char *s)
{
    if (!s)
        return;
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r' || s[len - 1] == ' ' || s[len - 1] == '\t'))
        s[--len] = '\0';
}

static int connect_local_socket(void)
{
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0)
        return -1;
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        close(sock);
        return -1;
    }
    return sock;
}

int is_builtin(char **args)
{
    if (args[0] == NULL)
        return 0;
    const char *cmd = basename_cmd(args[0]);
    if (strcmp(cmd, "quit") == 0)
        return -1;
    if (strcmp(cmd, "setenv") == 0)
        return 1;
    if (strcmp(cmd, "printenv") == 0)
        return 1;
    if (strcmp(cmd, "login") == 0)
        return 1;
    if (strcmp(cmd, "logout") == 0)
        return 1;
    return 0;
}

int exec_builtin(char **args)
{
    const char *cmd = basename_cmd(args[0]);
    if (strcmp(cmd, "quit") == 0)
        exit(0);
    else if (strcmp(cmd, "setenv") == 0)
    {
        if (args[1] == NULL || args[2] == NULL)
            fprintf(stderr, "Usage: setenv [variable] [value]\n");
        else
        {
            if (setenv(args[1], args[2], 1) != 0)
                perror("setenv");
        }
        return 0;
    }
    else if (strcmp(cmd, "printenv") == 0)
    {
        if (args[1] == NULL)
        {
            extern char **environ;
            for (char **env = environ; *env != NULL; env++)
                printf("%s\n", *env);
        }
        else
        {
            char *val = getenv(args[1]);
            if (val != NULL)
                printf("%s\n", val);
        }
        return 0;
    }
    else if (strcmp(cmd, "login") == 0)
    {
        char username[256];
        char password[256];

        printf("Login: ");
        fflush(stdout);
        if (fgets(username, sizeof(username), stdin) == NULL)
            return 1;
        username[strcspn(username, "\r\n")] = '\0';

        printf("Password: ");
        fflush(stdout);
        if (fgets(password, sizeof(password), stdin) == NULL)
            return 1;
        password[strcspn(password, "\r\n")] = '\0';

        int sock = connect_local_socket();
        if (sock < 0)
        {
            perror("connect");
            return 1;
        }

        Request req = {0};
        req.cmd = CMD_LOGIN;
        req.sender_id = atoi(getenv("USER_ID") ? getenv("USER_ID") : "0");
        strcpy(req.arg1, username);
        strcpy(req.arg2, password);
        write(sock, &req, sizeof(req));

        char response[256];
        int n = read(sock, response, sizeof(response) - 1);
        close(sock);
        if (n > 0)
        {
            response[n] = '\0';
            trim_whitespace(response);
            if (strcmp(response, "OK") == 0)
            {
                setenv("USER_NAME", username, 1);
                printf("Login successful!\n");
                return 0;
            }
            if (strcmp(response, "PASSWORD_ERROR") == 0)
            {
                printf("Password error !\n");
                return 1;
            }
            if (strcmp(response, "USER_NOT_FOUND") == 0)
            {
                printf("User not found !\n");
                return 1;
            }
        }
        return 1;
    }
    else if (strcmp(cmd, "logout") == 0)
    {
        char *username = getenv("USER_NAME");
        if (!username)
        {
            fprintf(stderr, "Not logged in.\n");
            return 1;
        }
        int sock = connect_local_socket();
        if (sock < 0)
        {
            perror("connect");
            return 1;
        }
        Request req = {0};
        req.cmd = CMD_LOGOUT;
        req.sender_id = atoi(getenv("USER_ID") ? getenv("USER_ID") : "0");
        strcpy(req.arg1, username);
        write(sock, &req, sizeof(req));
        close(sock);
        unsetenv("USER_NAME");
        printf("Logged out successfully!\n");
        return -1;
    }
    return 0;
}

int group_exists(const char *group_name)
{
    char q[256];
    snprintf(q, sizeof(q), "SELECT 1 FROM `groups` WHERE name='%s'", group_name);
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

int is_group_owner(const char *group_name, const char *owner)
{
    char q[256];
    snprintf(q, sizeof(q), "SELECT 1 FROM `groups` WHERE name='%s' AND owner='%s'", group_name, owner);
    pthread_mutex_lock(&db_mutex);
    if (mysql_query(db_conn, q))
    {
        pthread_mutex_unlock(&db_mutex);
        return 0;
    }
    MYSQL_RES *res = mysql_store_result(db_conn);
    int ok = (res && mysql_num_rows(res) > 0);
    if (res)
        mysql_free_result(res);
    pthread_mutex_unlock(&db_mutex);
    return ok;
}

int is_member_of_group(const char *group_name, const char *username)
{
    char q[256];
    snprintf(q, sizeof(q), "SELECT 1 FROM group_members WHERE group_name='%s' AND username='%s'", group_name, username);
    pthread_mutex_lock(&db_mutex);
    if (mysql_query(db_conn, q))
    {
        pthread_mutex_unlock(&db_mutex);
        return 0;
    }
    MYSQL_RES *res = mysql_store_result(db_conn);
    int ok = (res && mysql_num_rows(res) > 0);
    if (res)
        mysql_free_result(res);
    pthread_mutex_unlock(&db_mutex);
    return ok;
}

int create_group(const char *group_name, const char *owner)
{
    if (group_exists(group_name))
        return 0;
    char q[512];
    snprintf(q, sizeof(q), "INSERT INTO `groups` (name, owner) VALUES ('%s', '%s')", group_name, owner);
    pthread_mutex_lock(&db_mutex);
    int ret = mysql_query(db_conn, q);
    if (ret == 0)
    {
        char q2[512];
        snprintf(q2, sizeof(q2), "INSERT INTO group_members (group_name, username) VALUES ('%s', '%s')", group_name, owner);
        mysql_query(db_conn, q2);
    }
    pthread_mutex_unlock(&db_mutex);
    return (ret == 0);
}

int delete_group(const char *group_name, const char *owner)
{
    if (!group_exists(group_name))
        return 0;
    if (!is_group_owner(group_name, owner))
        return 0;
    char q[256];
    snprintf(q, sizeof(q), "DELETE FROM group_members WHERE group_name='%s'", group_name);
    pthread_mutex_lock(&db_mutex);
    mysql_query(db_conn, q);
    snprintf(q, sizeof(q), "DELETE FROM `groups` WHERE name='%s'", group_name);
    int ret = mysql_query(db_conn, q);
    pthread_mutex_unlock(&db_mutex);
    return (ret == 0);
}

int add_to_group(const char *group_name, const char *owner, const char *username)
{
    if (!group_exists(group_name))
        return 0;
    if (!is_group_owner(group_name, owner))
        return 0;
    if (!user_exists(username))
        return 0;
    if (is_member_of_group(group_name, username))
        return 0;
    char q[512];
    snprintf(q, sizeof(q), "INSERT INTO group_members (group_name, username) VALUES ('%s', '%s')", group_name, username);
    pthread_mutex_lock(&db_mutex);
    int ret = mysql_query(db_conn, q);
    pthread_mutex_unlock(&db_mutex);
    return (ret == 0);
}

int leave_group(const char *group_name, const char *username)
{
    if (!group_exists(group_name))
        return 0;
    if (!is_member_of_group(group_name, username))
        return 0;
    char q[256];
    snprintf(q, sizeof(q), "DELETE FROM group_members WHERE group_name='%s' AND username='%s'", group_name, username);
    pthread_mutex_lock(&db_mutex);
    int ret = mysql_query(db_conn, q);
    pthread_mutex_unlock(&db_mutex);
    return (ret == 0);
}

char *list_groups(const char *username)
{
    char q[256];
    snprintf(q, sizeof(q), "SELECT group_name FROM group_members WHERE username='%s'", username);
    pthread_mutex_lock(&db_mutex);
    if (mysql_query(db_conn, q))
    {
        pthread_mutex_unlock(&db_mutex);
        return strdup("Empty !\n");
    }
    MYSQL_RES *res = mysql_store_result(db_conn);
    if (!res || mysql_num_rows(res) == 0)
    {
        if (res)
            mysql_free_result(res);
        pthread_mutex_unlock(&db_mutex);
        return strdup("Empty !\n");
    }
    char *result = malloc(4096);
    result[0] = '\0';
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)))
    {
        strcat(result, row[0]);
        strcat(result, "\n");
    }
    mysql_free_result(res);
    pthread_mutex_unlock(&db_mutex);
    return result;
}

int remove_from_group(const char *group_name, const char *owner, const char *username)
{
    if (!group_exists(group_name))
        return 0;
    if (!is_group_owner(group_name, owner))
        return 0;
    if (!user_exists(username))
        return 0;
    if (!is_member_of_group(group_name, username))
        return 0;
    char q[256];
    snprintf(q, sizeof(q), "DELETE FROM group_members WHERE group_name='%s' AND username='%s'", group_name, username);
    pthread_mutex_lock(&db_mutex);
    int ret = mysql_query(db_conn, q);
    pthread_mutex_unlock(&db_mutex);
    return (ret == 0);
}

void gyell_to_group(const char *group_name, const char *sender, const char *msg)
{
    if (!group_exists(group_name))
        return;
    char q[256];
    snprintf(q, sizeof(q), "SELECT username FROM group_members WHERE group_name='%s'", group_name);
    pthread_mutex_lock(&db_mutex);
    if (mysql_query(db_conn, q))
    {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    MYSQL_RES *res = mysql_store_result(db_conn);
    if (!res)
    {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "<%s:%s>: %s", group_name, sender, msg);
    pthread_mutex_lock(&user_mutex);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)))
    {
        const char *uname = row[0];
        for (int i = 0; i < user_count; i++)
        {
            if (users[i].tcp_fd != -1 && strcmp(users[i].name, uname) == 0)
            {
                write(users[i].msg_fd, buffer, strlen(buffer));
                write(users[i].msg_fd, "\n", 1);
                break;
            }
        }
    }
    pthread_mutex_unlock(&user_mutex);
    mysql_free_result(res);
    pthread_mutex_unlock(&db_mutex);
}

int delete_mail(int mail_id, const char *username)
{
    /* Treat mail_id as 1-based index in the user's mailbox (ordered by sent_time).
       Find the actual DB id at that position, then delete that row. */
    if (mail_id <= 0)
        return 0;
    char q[256];
    /* Find the DB id for the given index (offset = mail_id - 1) */
    snprintf(q, sizeof(q), "SELECT id FROM mails WHERE to_user='%s' ORDER BY sent_time LIMIT 1 OFFSET %d", username, mail_id - 1);
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
    int real_id = atoi(row[0]);
    mysql_free_result(res);

    snprintf(q, sizeof(q), "DELETE FROM mails WHERE id=%d AND to_user='%s'", real_id, username);
    int ret = mysql_query(db_conn, q);
    int affected = mysql_affected_rows(db_conn);
    pthread_mutex_unlock(&db_mutex);
    return (ret == 0 && affected > 0);
}

/* ================ Previous ================ */
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
            {
                users[i] = users[user_count - 1];

                client_fd_to_user_index[users[i].tcp_fd] = i;
                msg_fd_to_user_index[users[i].msg_fd] = i;
            }
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
    /* ================ HW3 ================ */
    case CMD_LOGIN:
    {
        if (!user_exists(req.arg1))
            snprintf(response, BUFFER_SIZE, "USER_NOT_FOUND");
        else if (!verify_password(req.arg1, req.arg2))
            snprintf(response, BUFFER_SIZE, "PASSWORD_ERROR");
        else
        {
            pthread_mutex_lock(&user_mutex);
            int idx = -1;
            if (req.sender_id > 0)
            {
                for (int i = 0; i < user_count; i++)
                {
                    if (users[i].id == req.sender_id && users[i].tcp_fd != -1)
                    {
                        idx = i;
                        break;
                    }
                }
            }
            if (idx >= 0)
            {
                strncpy(users[idx].name, req.arg1, MAX_NAME - 1);
                users[idx].name[MAX_NAME - 1] = '\0';
            }
            pthread_mutex_unlock(&user_mutex);
            snprintf(response, BUFFER_SIZE, "OK");
        }
        break;
    }
    case CMD_REGISTER:
    {
        int ret = register_user(req.arg1, req.arg2);
        if (ret == 1)
        {
            snprintf(response, BUFFER_SIZE, "OK");
        }
        else
        {
            snprintf(response, BUFFER_SIZE, "Register failed!");
        }
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
    /* ================ HW3 Group ================ */
    case CMD_CREATEGROUP:
    {
        const char *owner = (req.arg2[0] != '\0') ? req.arg2 : NULL;
        if (!owner || !user_exists(owner))
            snprintf(response, BUFFER_SIZE, "User not found.\n");
        else if (group_exists(req.arg1))
            snprintf(response, BUFFER_SIZE, "Group already exist !\n");
        else if (create_group(req.arg1, owner))
            snprintf(response, BUFFER_SIZE, "Group create success!\n");
        else
            snprintf(response, BUFFER_SIZE, "Group create failed!\n");
        break;
    }
    case CMD_DELGROUP:
    {
        const char *owner = (req.arg2[0] != '\0') ? req.arg2 : NULL;
        if (!owner || !user_exists(owner))
            snprintf(response, BUFFER_SIZE, "User not found.\n");
        else if (!group_exists(req.arg1))
            snprintf(response, BUFFER_SIZE, "Group not found !\n");
        else if (!is_group_owner(req.arg1, owner))
            snprintf(response, BUFFER_SIZE, "You don't have permission !\n");
        else if (delete_group(req.arg1, owner))
            snprintf(response, BUFFER_SIZE, "Group delete success !\n");
        else
            snprintf(response, BUFFER_SIZE, "Delete failed!\n");
        break;
    }
    case CMD_ADDTO:
    {
        const char *owner = (req.arg3[0] != '\0') ? req.arg3 : NULL;
        if (!owner || !user_exists(owner))
            snprintf(response, BUFFER_SIZE, "User not found.\n");
        else if (!group_exists(req.arg1))
            snprintf(response, BUFFER_SIZE, "Group not found !\n");
        else if (!is_group_owner(req.arg1, owner))
            snprintf(response, BUFFER_SIZE, "You don't have permission !\n");
        else
        {
            char userlist[1024];
            strcpy(userlist, req.arg2);
            char *saveptr;
            char *token = strtok_r(userlist, ",", &saveptr);
            char success_list[1024] = {0};
            char fail_notfound[1024] = {0};
            char fail_already[1024] = {0};

            while (token)
            {
                if (!user_exists(token))
                {
                    strcat(fail_notfound, token);
                    strcat(fail_notfound, " ");
                }
                else if (is_member_of_group(req.arg1, token))
                {
                    strcat(fail_already, token);
                    strcat(fail_already, " ");
                }
                else
                {
                    add_to_group(req.arg1, owner, token);
                    strcat(success_list, token);
                    strcat(success_list, " ");
                }
                token = strtok_r(NULL, ",", &saveptr);
            }
            if (strlen(success_list) > 0)
            {
                char tmp[BUFFER_SIZE];
                snprintf(tmp, sizeof(tmp), "%sadd success !\n", success_list);
                strcat(response, tmp);
            }
            if (strlen(fail_notfound) > 0)
            {
                char tmp[BUFFER_SIZE];
                snprintf(tmp, sizeof(tmp), "%snot found !\n", fail_notfound);
                strcat(response, tmp);
            }
            if (strlen(fail_already) > 0)
            {
                char tmp[BUFFER_SIZE];
                snprintf(tmp, sizeof(tmp), "%salready in group !\n", fail_already);
                strcat(response, tmp);
            }
            if (strlen(response) == 0)
                strcpy(response, "No valid user.\n");
        }
        break;
    }
    case CMD_LEAVEGROUP:
    {
        const char *username = (req.arg2[0] != '\0') ? req.arg2 : NULL;
        if (!username || !user_exists(username))
            snprintf(response, BUFFER_SIZE, "User not found.\n");
        else if (!group_exists(req.arg1))
            snprintf(response, BUFFER_SIZE, "Group not found !\n");
        else if (!is_member_of_group(req.arg1, username))
            snprintf(response, BUFFER_SIZE, "You are not a member of this group.\n");
        else if (leave_group(req.arg1, username))
            snprintf(response, BUFFER_SIZE, "Leave success !\n");
        else
            snprintf(response, BUFFER_SIZE, "Leave fault !\n");
        break;
    }
    case CMD_LISTGROUP:
    {
        char *result = list_groups(req.arg1);
        snprintf(response, BUFFER_SIZE, "%s", result);
        free(result);
        break;
    }
    case CMD_REMOVE:
    {
        const char *owner = (req.arg3[0] != '\0') ? req.arg3 : NULL;
        if (!owner || !user_exists(owner))
            snprintf(response, BUFFER_SIZE, "User not found.\n");
        else if (!group_exists(req.arg1))
            snprintf(response, BUFFER_SIZE, "Group not found !\n");
        else if (!is_group_owner(req.arg1, owner))
            snprintf(response, BUFFER_SIZE, "You don't have permission !\n");
        else
        {
            char userlist[1024];
            strcpy(userlist, req.arg2);
            char *saveptr;
            char *token = strtok_r(userlist, " ", &saveptr);
            char success_list[1024] = {0};
            char fail_notfound[1024] = {0};
            char fail_notingroup[1024] = {0};

            while (token)
            {
                if (!user_exists(token))
                {
                    strcat(fail_notfound, token);
                    strcat(fail_notfound, " ");
                }
                else if (!is_member_of_group(req.arg1, token))
                {
                    strcat(fail_notingroup, token);
                    strcat(fail_notingroup, " ");
                }
                else
                {
                    remove_from_group(req.arg1, owner, token);
                    strcat(success_list, token);
                    strcat(success_list, " ");
                }
                token = strtok_r(NULL, ",", &saveptr);
            }
            if (strlen(success_list) > 0)
            {
                char tmp[BUFFER_SIZE];
                snprintf(tmp, sizeof(tmp), "%sremove success !\n", success_list);
                strcat(response, tmp);
            }
            if (strlen(fail_notfound) > 0)
            {
                char tmp[BUFFER_SIZE];
                snprintf(tmp, sizeof(tmp), "%snot found !\n", fail_notfound);
                strcat(response, tmp);
            }
            if (strlen(fail_notingroup) > 0)
            {
                char tmp[BUFFER_SIZE];
                snprintf(tmp, sizeof(tmp), "%sis not in group !\n", fail_notingroup);
                strcat(response, tmp);
            }
            if (strlen(response) == 0)
                strcpy(response, "No valid user.\n");
        }
        break;
    }
    case CMD_GYELL:
    {
        User *self = find_user_by_id(req.sender_id);
        if (!self)
            snprintf(response, BUFFER_SIZE, "User not found.\n");
        else if (!group_exists(req.arg1))
            snprintf(response, BUFFER_SIZE, "Group not found !\n");
        else if (!is_member_of_group(req.arg1, self->name))
            snprintf(response, BUFFER_SIZE, "You are not a member of this group.\n");
        else
        {
            gyell_to_group(req.arg1, self->name, req.arg2);
            snprintf(response, BUFFER_SIZE, "gyell accept!\n");
        }
        break;
    }
    case CMD_LOGOUT:
    {
        pthread_mutex_lock(&user_mutex);
        int idx = -1;
        if (req.sender_id > 0)
        {
            for (int i = 0; i < user_count; i++)
            {
                if (users[i].id == req.sender_id && users[i].tcp_fd != -1)
                {
                    idx = i;
                    break;
                }
            }
        }
        if (idx == -1)
        {
            for (int i = 0; i < user_count; i++)
            {
                if (users[i].tcp_fd != -1 && strcmp(users[i].name, req.arg1) == 0)
                {
                    idx = i;
                    break;
                }
            }
        }
        if (idx != -1)
            snprintf(users[idx].name, MAX_NAME, "user%d", users[idx].id);
        pthread_mutex_unlock(&user_mutex);
        snprintf(response, BUFFER_SIZE, "OK");
        break;
    }
    /* ================ HW2 ================ */
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

                char user_id_str[16];
                snprintf(user_id_str, sizeof(user_id_str), "%d", id);
                setenv("USER_ID", user_id_str, 1);

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
