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

    printf("your user name: ");
    fflush(stdout);
    if (fgets(username, sizeof(username), stdin) == NULL)
        return 1;
    username[strcspn(username, "\r\n")] = '\0';

    printf("your password: ");
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
    req.cmd = CMD_REGISTER;
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
            printf("Create success !\n");
            return 0;
        }
        if (strcmp(response, "USER_EXISTS") == 0)
        {
            printf("User name already exist !\n");
            return 1;
        }
    }
    printf("Register failed!\n");
    return 1;
}
