#include "../include/shell.h"

int main() {
    char username[256];
    char password[256];
    
    printf("your user name: ");
    fflush(stdout);
    if (fgets(username, sizeof(username), stdin) == NULL) return 1;
    username[strcspn(username, "\r\n")] = '\0';
    
    printf("your password: ");
    fflush(stdout);
    if (fgets(password, sizeof(password), stdin) == NULL) return 1;
    password[strcspn(password, "\r\n")] = '\0';
    
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
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
    
    if (n > 0 && strcmp(response, "OK") == 0) {
        printf("Create success !\n");
        return 0;
    } else {
        printf("Register failed!\n");
        return 1;
    }
}