#include "../include/shell.h"

int main() {
    char *username = getenv("USER_NAME");
    if (!username) {
        fprintf(stderr, "Not logged in.\n");
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
    
    Request req = {0};
    req.cmd = CMD_LOGOUT;
    strcpy(req.arg1, username);
    
    write(sock, &req, sizeof(req));
    close(sock);
    
    // 清除環境變數
    unsetenv("USER_NAME");
    printf("Logged out successfully!\n");
    return 0;
}