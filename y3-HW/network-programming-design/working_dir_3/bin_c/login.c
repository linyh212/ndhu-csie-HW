#include "../include/shell.h"

int main() {
    char username[256];
    char password[256];
    
    printf("Login: ");
    fflush(stdout);
    if (fgets(username, sizeof(username), stdin) == NULL) return 1;
    username[strcspn(username, "\r\n")] = '\0';
    
    printf("Password: ");
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
    req.cmd = CMD_LOGIN;
    strcpy(req.arg1, username);
    strcpy(req.arg2, password);
    
    write(sock, &req, sizeof(req));
    
    char response[256];
    int n = read(sock, response, sizeof(response) - 1);
    close(sock);
    
    if (n > 0) {
        response[n] = '\0';
        // bin_c/login.c - 登入成功後執行 shell
        if (strcmp(response, "OK") == 0) {
            setenv("USER_NAME", username, 1);
            // 執行 shell
            execl("./shell", "shell", NULL);
            perror("execl");
            return 1;
        } else if (strcmp(response, "PASSWORD_ERROR") == 0) {
            printf("Password error !\n");
            return 1;
        } else if (strcmp(response, "USER_NOT_FOUND") == 0) {
            printf("User not found !\nCreate account or login again ? <1/2>: ");
            fflush(stdout);
            char choice[10];
            fgets(choice, sizeof(choice), stdin);
            if (choice[0] == '1') {
                execl("./bin/register", "register", NULL);
            }
            return 1;
        }
    }
    return 1;
}