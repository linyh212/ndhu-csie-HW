#include "shell.h"

int execute_line(char *line)
{
    char line_copy[MAX_LINE];
    strcpy(line_copy, line);

    int numbered_pipe_fd = -1;
    char *last_pipe = strrchr(line_copy, '|');
    int has_numbered_pipe = 0;
    if (last_pipe != NULL && last_pipe[1] != '\0')
    {
        char *num_start = last_pipe + 1;
        char *endptr;
        long n = strtol(num_start, &endptr, 10);
        if (*endptr == '\0' && n >= 1 && n <= 128)
        {
            *last_pipe = '\0';
            numbered_pipe_fd = create_numbered_pipe((int)n);
            has_numbered_pipe = 1;
        }
    }

    int input_fd = STDIN_FILENO;
    if (!has_numbered_pipe)
    {
        input_fd = get_input_from_num_pipe();
        if (input_fd == -1)
            input_fd = STDIN_FILENO;
    }

    if (strchr(line_copy, '|') != NULL)
    {
        char *commands[MAX_CMDS];
        int cmd_count = parse_commands(line_copy, commands);
        if (cmd_count > 1)
            return execute_pipeline(commands, cmd_count);
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
        return 0;
    int output_fd = (numbered_pipe_fd != -1) ? numbered_pipe_fd : STDOUT_FILENO;
    int ret;

    if (is_builtin(args))
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
            {
                wait(NULL);
            }
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

int main()
{
    char commandStr[MAX_LINE] = {0};
    setenv("PATH", "bin:.", 1);
    init_numbered_pipes();
    do
    {
        printf("MyShell%% ");
        fflush(stdout);
        if (fgets(commandStr, sizeof(commandStr), stdin) == NULL)
            break;
        commandStr[strcspn(commandStr, "\n")] = '\0';
        update_num_pipes();
        if (execute_line(commandStr) == -1)
            break;
    } while (1);

    return 0;
}