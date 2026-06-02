#include "../include/shell.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: mailto <user_name> <message>\n");
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
    if (strcmp(argv[2], "<") == 0)
    {
        if (argc > 3)
        {
            char command[512] = {0};
            for (int i = 3; i < argc; i++)
            {
                if (i > 3)
                    strncat(command, " ", sizeof(command) - strlen(command) - 1);
                strncat(command, argv[i], sizeof(command) - strlen(command) - 1);
            }
            FILE *fp = popen(command, "r");
            if (fp)
            {
                size_t len = fread(message, 1, sizeof(message) - 1, fp);
                message[len] = '\0';
                pclose(fp);
            }
        }
        else
        {
            size_t len = fread(message, 1, sizeof(message) - 1, stdin);
            message[len] = '\0';
        }
    }
    else
    {
        for (int i = 2; i < argc; i++)
        {
            if (i > 2)
                strncat(message, " ", sizeof(message) - strlen(message) - 1);
            strncat(message, argv[i], sizeof(message) - strlen(message) - 1);
        }
    }

    Request req = {0};
    req.cmd = CMD_MAILTO;
    strcpy(req.arg1, argv[1]);   // to
    strcpy(req.arg2, message);   // message
    strcpy(req.arg3, username);  // from

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
