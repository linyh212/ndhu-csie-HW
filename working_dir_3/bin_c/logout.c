#include "../include/shell.h"

int main()
{
    char *username = getenv("USER_NAME");
    if (!username)
    {
        fprintf(stderr, "Not logged in.\n");
        return 1;
    }
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        return 1;
    }
    Request req = {0};
    req.cmd = CMD_LOGOUT;
    req.sender_id = atoi(getenv("USER_ID") ? getenv("USER_ID") : "0");
    strcpy(req.arg1, username);
    write(sock, &req, sizeof(req));
    char resp[256] = {0};
    read(sock, resp, sizeof(resp) - 1);
    close(sock);
    if (strcmp(resp, "OK") == 0)
    {
        printf("Logged out successfully!\n");
        return 99;
    }
    return 1;
}
