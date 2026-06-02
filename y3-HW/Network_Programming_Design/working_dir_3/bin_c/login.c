#include "../include/shell.h"

static int read_secret(char *buf, size_t size)
{
    struct termios oldt;
    int hide_echo = (isatty(STDIN_FILENO) && tcgetattr(STDIN_FILENO, &oldt) == 0);
    if (hide_echo)
    {
        struct termios newt = oldt;
        newt.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &newt);
    }
    char *ret = fgets(buf, size, stdin);
    if (hide_echo)
    {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldt);
        printf("\n");
    }
    if (ret == NULL)
        return 0;
    buf[strcspn(buf, "\r\n")] = '\0';
    return 1;
}

int main()
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
    if (!read_secret(password, sizeof(password)))
        return 1;

    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
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
        size_t len = strlen(response);
        while (len > 0 && (response[len - 1] == '\n' || response[len - 1] == '\r' || response[len - 1] == ' ' || response[len - 1] == '\t'))
            response[--len] = '\0';
        if (strcmp(response, "OK") == 0)
        {
            setenv("USER_NAME", username, 1);
            execl("./shell", "shell", NULL);
            perror("execl");
            return 1;
        }
        else if (strcmp(response, "PASSWORD_ERROR") == 0)
        {
            printf("Password error !\n");
            return 1;
        }
        else if (strcmp(response, "USER_NOT_FOUND") == 0)
        {
            printf("User not found !\nCreate account or login again ? <1/2>: ");
            fflush(stdout);
            char choice[10];
            fgets(choice, sizeof(choice), stdin);
            if (choice[0] == '1')
            {
                execl("./bin/register", "register", NULL);
            }
            return 1;
        }
    }
    return 1;
}
