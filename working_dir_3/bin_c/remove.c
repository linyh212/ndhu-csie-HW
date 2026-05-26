#include "../include/shell.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: remove <group_name> <user_name1> [user_name2] ...\n");
        return 1;
    }
    
    char *username = getenv("USER_NAME");
    if (!username) {
        fprintf(stderr, "Not logged in. Please login first.\n");
        return 1;
    }
    
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        return 1;
    }
    
    // 構建使用者列表字串
    char userlist[1024] = {0};
    for (int i = 2; i < argc; i++) {
        strcat(userlist, argv[i]);
        if (i < argc - 1) strcat(userlist, ",");
    }
    
    Request req = {0};
    req.cmd = CMD_REMOVE;
    strcpy(req.arg1, argv[1]);      // group_name
    strcpy(req.arg2, userlist);     // user list
    strcpy(req.arg3, username);     // owner
    
    write(sock, &req, sizeof(req));
    
    char response[4096];
    int n = read(sock, response, sizeof(response) - 1);
    close(sock);
    
    if (n > 0) {
        response[n] = '\0';
        printf("%s", response);
    }
    return 0;
}