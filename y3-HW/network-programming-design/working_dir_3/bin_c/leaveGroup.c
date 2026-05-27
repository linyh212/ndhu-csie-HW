#include "../include/shell.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: leaveGroup <group_name>\n");
        return 1;
    }

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
    req.cmd = CMD_LEAVEGROUP;
    strcpy(req.arg1, argv[1]);  // group_name
    strcpy(req.arg2, username); // user

    write(sock, &req, sizeof(req));

    char response[256];
    int n = read(sock, response, sizeof(response) - 1);
    close(sock);

    if (n > 0)
    {
        response[n] = '\0';
        printf("%s", response);
    }
    return 0;
}