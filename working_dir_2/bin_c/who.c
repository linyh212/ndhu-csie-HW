#include "../include/shell.h"

int main(int argc, char *argv[])
{
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
    req.cmd = CMD_WHO;
    req.sender_id = atoi(getenv("USER_ID") ? getenv("USER_ID") : "0");
    write(sock, &req, sizeof(req));
    char buf[BUFFER_SIZE];
    int n = read(sock, buf, sizeof(buf) - 1);
    if (n > 0)
    {
        buf[n] = '\0';
        printf("%s", buf);
    }
    close(sock);
    return 0;
}