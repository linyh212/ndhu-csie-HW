#include "../include/shell.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: gyell <group_name> <message>\n");
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

    char message[1024] = {0};
    for (int i = 2; i < argc; i++)
    {
        strcat(message, argv[i]);
        if (i < argc - 1)
            strcat(message, " ");
    }

    Request req = {0};
    req.cmd = CMD_GYELL;
    req.sender_id = atoi(getenv("USER_ID") ? getenv("USER_ID") : "0");
    strcpy(req.arg1, argv[1]);  // group_name
    strcpy(req.arg2, message);  // message

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