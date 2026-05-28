#include "shell.h"
#include <sys/select.h>
#include <sys/un.h>
#include <signal.h>

User users[MAX_USER];
int user_count = 0;
pthread_mutex_t user_mutex = PTHREAD_MUTEX_INITIALIZER;
int listen_fd, local_fd;

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
    for (int i = 0; i < MAX_USER; i++)
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
    users[user_count].id = id;
    strncpy(users[user_count].name, name, MAX_NAME - 1);
    users[user_count].name[MAX_NAME - 1] = '\0';
    users[user_count].tcp_fd = tcp_fd;
    users[user_count].msg_fd = msg_fd;
    user_count++;
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
                close(users[i].msg_fd);
            if (users[i].tcp_fd != -1)
                close(users[i].tcp_fd);
            printf("User disconnected: ID=%d, name=%s\n", users[i].id, users[i].name);
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
    case CMD_WHO:
    {
        for (int id = 0; id < MAX_USER; id++)
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
    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, sigchld_handler);

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
                char user_id_str[16];
                snprintf(msg_fd_str, sizeof(msg_fd_str), "%d", sv[0]);
                snprintf(user_id_str, sizeof(user_id_str), "%d", id);
                setenv("MESSAGE_FD", msg_fd_str, 1);
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
                char welcome[256];
                snprintf(welcome, sizeof(welcome), "Welcome! Your ID is %d, name is %s\n", id, default_name);
                write(sv[1], welcome, strlen(welcome));
                printf("New user connected: ID=%d, name=%s\n", id, default_name);
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
                if (ret == 0)
                {
                    disconnected_fds[discon_count++] = users[i].tcp_fd;
                }
                else if (ret < 0 && errno != EAGAIN && errno != EINTR)
                {
                    disconnected_fds[discon_count++] = users[i].tcp_fd;
                }
            }
        }
        pthread_mutex_unlock(&user_mutex);

        for (int i = 0; i < discon_count; i++)
        {
            printf("User disconnected (fd=%d)\n", disconnected_fds[i]);
            remove_user(disconnected_fds[i]);
        }
    }
    close(listen_fd);
    close(local_fd);
    return 0;
}