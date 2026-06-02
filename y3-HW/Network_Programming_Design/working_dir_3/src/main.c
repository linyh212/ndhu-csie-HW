#include "shell.h"

static void trim_whitespace(char *s)
{
    if (!s)
        return;
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r' || s[len - 1] == ' ' || s[len - 1] == '\t'))
        s[--len] = '\0';
}

static int read_secret(char *buf, size_t size)
{
    struct termios oldt;
    int hide_echo = (isatty(STDIN_FILENO) && tcgetattr(STDIN_FILENO, &oldt) == 0);
    if (hide_echo)
    {
        struct termios newt = oldt;
        newt.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &newt);
    }
    char *ret = fgets(buf, size, stdin);
    if (hide_echo)
    {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldt);
        printf("\n");
    }
    if (ret == NULL)
        return 0;
    buf[strcspn(buf, "\r\n")] = '\0';
    return 1;
}

static int should_read_from_numbered_pipe(char *cmd)
{
    for (int i = 0; i < MAX_NPIPES; i++)
    {
        if (npipes[i].used && npipes[i].remain == 0)
            return 1;
    }
    return 0;
}

static int is_valid_command_line(const char *line)
{
    char line_copy[MAX_LINE];
    strcpy(line_copy, line);
    char *last_pipe = strrchr(line_copy, '|');
    if (last_pipe != NULL && last_pipe[1] != '\0')
    {
        char *num_start = last_pipe + 1;
        char *endptr;
        long n = strtol(num_start, &endptr, 10);
        if (*endptr == '\0' && n >= 1 && n <= 128)
            *last_pipe = '\0';
    }
    char *commands[MAX_CMDS];
    int cmd_count = parse_commands(line_copy, commands);
    for (int i = 0; i < cmd_count; i++)
    {
        char cmd_copy[MAX_CMD];
        strcpy(cmd_copy, commands[i]);
        char *args[MAX_ARGS];
        parse_args(cmd_copy, args);
        if (args[0] == NULL)
            return 0;
        int builtin_type = is_builtin(args);
        if (builtin_type != 0)
            continue;
        char *fullpath = search_path(args[0]);
        if (fullpath == NULL)
            return 0;
        free(fullpath);
    }
    return 1;
}

int execute_line(char *line)
{
    line[strcspn(line, "\r\n")] = '\0';
    char line_copy[MAX_LINE];
    char original_line[MAX_LINE];
    strcpy(line_copy, line);
    strcpy(original_line, line);
    int numbered_pipe_fd = -1;
    int line_valid = is_valid_command_line(line);
    char *last_pipe = strrchr(line_copy, '|');
    if (last_pipe != NULL && last_pipe[1] != '\0')
    {
        char *num_start = last_pipe + 1;
        char *endptr;
        long n = strtol(num_start, &endptr, 10);
        if (*endptr == '\0' && n >= 1 && n <= 128)
        {
            *last_pipe = '\0';
            if (line_valid)
                numbered_pipe_fd = create_numbered_pipe((int)n);
        }
    }
    char *trimmed = line_copy;
    while (*trimmed == ' ' || *trimmed == '\t')
        trimmed++;
    if (*trimmed == '\0')
    {
        if (numbered_pipe_fd != -1)
            close(numbered_pipe_fd);
        return 0;
    }
    if (strchr(line_copy, '|') != NULL)
    {
        char *commands[MAX_CMDS];
        int cmd_count = parse_commands(line_copy, commands);
        if (cmd_count > 1)
        {
            int input_fd = STDIN_FILENO;
            if (line_valid && should_read_from_numbered_pipe(commands[0]))
            {
                input_fd = get_input_from_num_pipe();
                if (input_fd == -1)
                    input_fd = STDIN_FILENO;
            }
            int ret = execute_pipeline(commands, cmd_count, input_fd, numbered_pipe_fd);
            if (numbered_pipe_fd != -1)
                close(numbered_pipe_fd);
            return ret;
        }
    }
    char *args[MAX_ARGS];
    int i = 0;
    char *token = strtok(line_copy, " \t");
    while (token && i < MAX_ARGS - 1)
    {
        args[i++] = token;
        token = strtok(NULL, " \t");
    }
    args[i] = NULL;
    if (args[0] == NULL)
    {
        if (numbered_pipe_fd != -1)
            close(numbered_pipe_fd);
        return 0;
    }
    int needs_pipe_input = (line_valid) ? should_read_from_numbered_pipe(args[0]) : 0;
    int input_fd = STDIN_FILENO;
    if (needs_pipe_input)
    {
        input_fd = get_input_from_num_pipe();
        if (input_fd == -1)
            input_fd = STDIN_FILENO;
    }
    int output_fd = (numbered_pipe_fd != -1) ? numbered_pipe_fd : STDOUT_FILENO;
    int ret;
    int builtin_type = is_builtin(args);
    if (builtin_type == -1)
        ret = exec_builtin(args);
    else if (builtin_type == 2)
        ret = exec_builtin(args);
    else if (builtin_type == 1)
    {
        if (input_fd != STDIN_FILENO || output_fd != STDOUT_FILENO)
        {
            pid_t pid = fork();
            if (pid == 0)
            {
                if (input_fd != STDIN_FILENO)
                    dup2(input_fd, STDIN_FILENO);
                if (output_fd != STDOUT_FILENO)
                    dup2(output_fd, STDOUT_FILENO);
                exec_builtin(args);
                exit(0);
            }
            else
                waitpid(pid, NULL, 0);
            ret = 0;
        }
        else
            ret = exec_builtin(args);
    }
    else
        ret = exec_external(args, input_fd, output_fd);
    if (numbered_pipe_fd != -1)
        close(numbered_pipe_fd);
    if (input_fd != STDIN_FILENO)
        close(input_fd);
    return ret;
}

void print_prompt(void) {
    char *user = getenv("USER_NAME");
    if (user && strlen(user) > 0)
        printf("(%s)%% ", user);
    else
        printf("%% ");
    fflush(stdout);
}

int main()
{
login_again:
    ;
    int quit_flag = 0;
    char commandStr[MAX_LINE] = {0};
    setenv("PATH", "bin:.", 1);
    init_numbered_pipes();
    /* ================ HW3 ================ */
    char username[256];
    char password[256];
    int logged_in = 0;
    while (!logged_in)
    {
        printf("Login: ");
        fflush(stdout);
        if (fgets(username, sizeof(username), stdin) == NULL)
            return 0;
        username[strcspn(username, "\r\n")] = '\0';
        printf("Password: ");
        fflush(stdout);
        if (!read_secret(password, sizeof(password)))
            return 0;
        int sock = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sock < 0)
        {
            perror("socket");
            continue;
        }
        struct sockaddr_un addr;
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, SOCKET_PATH);
        if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {
            perror("connect");
            close(sock);
            continue;
        }
        Request req = {0};
        req.cmd = CMD_LOGIN;
        req.sender_id = atoi(getenv("USER_ID") ? getenv("USER_ID") : "0");
        strcpy(req.arg1, username);
        strcpy(req.arg2, password);
        write(sock, &req, sizeof(req));
        char response[256];
        int n = read(sock, response, sizeof(response) - 1);
        close(sock);
        if (n > 0)
        {
            response[n] = '\0';
            if (strcmp(response, "OK") == 0)
            {
                setenv("USER_NAME", username, 1);
                printf("Login successful!\n");
                logged_in = 1;
                break;
            }
            else if (strcmp(response, "PASSWORD_ERROR") == 0)
                printf("Password error !\n");
            else if (strcmp(response, "USER_NOT_FOUND") == 0)
            {
                printf("User not found ! Create account or login again ? <1/2> :");
                fflush(stdout);
                char choice[10];
                fgets(choice, sizeof(choice), stdin);
                if (choice[0] == '1')
                {
                    while (1)
                    {
                        printf("your user name: ");
                        fflush(stdout);
                        fgets(username, sizeof(username), stdin);
                        username[strcspn(username, "\r\n")] = '\0';

                        printf("your password: ");
                        fflush(stdout);
                        if (!read_secret(password, sizeof(password)))
                            return 0;

                        sock = socket(AF_UNIX, SOCK_STREAM, 0);
                        if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
                        {
                            perror("connect");
                            close(sock);
                            break;
                        }
                        req.cmd = CMD_REGISTER;
                        strcpy(req.arg1, username);
                        strcpy(req.arg2, password);
                        write(sock, &req, sizeof(req));
                        n = read(sock, response, sizeof(response) - 1);
                        close(sock);
                        if (n > 0)
                        {
                            response[n] = '\0';
                            trim_whitespace(response);
                            if (strcmp(response, "OK") == 0)
                            {
                                printf("Create success !\n");
                                break;
                            }
                            if (strcmp(response, "USER_EXISTS") == 0)
                            {
                                printf("User name already exist !\n");
                                continue;
                            }
                            printf("Register failed!\n");
                            continue;
                        }
                    }
                }
            }
        }
    }
    /* ================ HW2 ================ */
    int msg_fd = -1;
    char *msg_fd_str = getenv("MESSAGE_FD");
    if (msg_fd_str)
    {
        msg_fd = atoi(msg_fd_str);
        fcntl(msg_fd, F_SETFL, O_NONBLOCK);
    }
    fd_set readfds;
    print_prompt();
    fflush(stdout);
    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        int maxfd = STDIN_FILENO;
        if (msg_fd != -1)
        {
            FD_SET(msg_fd, &readfds);
            if (msg_fd > maxfd)
                maxfd = msg_fd;
        }
        if (select(maxfd + 1, &readfds, NULL, NULL, NULL) < 0)
        {
            if (errno == EINTR)
                continue;
            break;
        }
        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            if (fgets(commandStr, sizeof(commandStr), stdin) == NULL)
                break;
            commandStr[strcspn(commandStr, "\r\n")] = '\0';

            char *trimmed = commandStr;
            while (*trimmed == ' ' || *trimmed == '\t')
                trimmed++;
            if (*trimmed == '\0')
            {
                update_num_pipes();
                print_prompt();
                fflush(stdout);
                continue;
            }
            int valid_line = is_valid_command_line(trimmed);
            if (valid_line)
                update_num_pipes();
            int ret = execute_line(commandStr);
            if (ret == -1)
            {
                if (msg_fd != -1)
                {
                    close(msg_fd);
                    msg_fd = -1;
                }
                quit_flag = 1;
                break;
            }
            if (ret == -2)
            {
                unsetenv("USER_NAME");
                close_all_numbered_pipes();
                goto login_again;
            }
            cleanup_zombies();
            print_prompt();
        }
        if (msg_fd != -1 && FD_ISSET(msg_fd, &readfds))
        {
            char msg[4096];
            int n = read(msg_fd, msg, sizeof(msg) - 1);
            if (n > 0)
            {
                msg[n] = '\0';
                if (n > 0 && msg[n - 1] == '\n')
                    msg[n - 1] = '\0';
                printf("\r%s\n", msg);
                print_prompt();
            }
            else if (n == 0)
            {
                close(msg_fd);
                msg_fd = -1;
            }
            else if (n < 0 && errno != EAGAIN && errno != EINTR)
            {
                close(msg_fd);
                msg_fd = -1;
            }
        }
    }
    if (!quit_flag)
        printf("\n");
    return 0;
}

void cleanup_zombies(void)
{
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
}
