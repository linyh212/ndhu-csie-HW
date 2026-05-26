#include "shell.h"

static const char *basename_cmd(char *cmd)
{
    if (cmd == NULL)
        return NULL;
    char *base = strrchr(cmd, '/');
    return base ? base + 1 : cmd;
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
        strcpy(req.arg1, username);
        strcpy(req.arg2, password);
        write(sock, &req, sizeof(req));

        char response[256];
        int n = read(sock, response, sizeof(response) - 1);
        close(sock);
        if (n > 0)
        {
            response[n] = '\0';
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
        strcpy(req.arg1, username);
        write(sock, &req, sizeof(req));
        close(sock);

        unsetenv("USER_NAME");
        printf("Logged out successfully!\n");
        return 0;
    }
    return 0;
}