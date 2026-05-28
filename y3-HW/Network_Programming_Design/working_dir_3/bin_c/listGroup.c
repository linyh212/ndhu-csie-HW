#include "../include/shell.h"

int main()
{
    char *username = getenv("USER_NAME");
    if (!username)
    {
        fprintf(stderr, "Not logged in. Please login first.\n");
        return 1;
    }

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
    req.cmd = CMD_LISTGROUP;
    strcpy(req.arg1, username);

    write(sock, &req, sizeof(req));

    char response[4096];
    int n = read(sock, response, sizeof(response) - 1);
    close(sock);

    if (n > 0)
    {
        response[n] = '\0';
        printf("%s", response);
    }
    return 0;
}