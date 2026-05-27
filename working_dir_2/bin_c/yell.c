#include "../include/shell.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: yell <message>\n");
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
    req.cmd = CMD_YELL;
    req.sender_id = atoi(getenv("USER_ID") ? getenv("USER_ID") : "0");
    size_t pos = 0;
    for (int i = 1; i < argc; i++)
    {
        size_t len = strlen(argv[i]);
        if (pos + len >= sizeof(req.arg1))
            break;
        memcpy(req.arg1 + pos, argv[i], len);
        pos += len;
        if (i < argc - 1 && pos < sizeof(req.arg1) - 1)
            req.arg1[pos++] = ' ';
    }
    req.arg1[pos] = '\0';
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