#include "shell.h"

int parse_commands(char *line, char *commands[])
{
    int cmd_count = 0;
    char *saveptr;
    char *token = strtok_r(line, "|", &saveptr);
    while (token != NULL && cmd_count < MAX_CMDS)
    {
        while (*token == ' ' || *token == '\t')
            token++;
        char *end = token + strlen(token) - 1;
        while (end > token && (*end == ' ' || *end == '\t'))
        {
            *end = '\0';
            end--;
        }
        commands[cmd_count++] = token;
        token = strtok_r(NULL, "|", &saveptr);
    }
    return cmd_count;
}

int parse_args(char *cmd_str, char *args[])
{
    int i = 0;
    char *saveptr;
    char *token = strtok_r(cmd_str, " \t", &saveptr);
    while (token != NULL && i < MAX_ARGS - 1)
    {
        args[i++] = token;
        token = strtok_r(NULL, " \t", &saveptr);
    }
    args[i] = NULL;
    return i;
}

int execute_pipeline(char **commands, int cmd_count)
{
    int pipe_fd[2];
    int input_fd = STDIN_FILENO;
    pid_t pids[MAX_CMDS];
    int status;
    int valid_commands = 0;

    for (int i = 0; i < cmd_count; i++)
    {
        char cmd_copy[MAX_CMD];
        strcpy(cmd_copy, commands[i]);
        char *args[MAX_ARGS];
        parse_args(cmd_copy, args);
        if (args[0] == NULL)
        {
            fprintf(stderr, "Invalid command\n");
            return -1;
        }
        if (i < cmd_count - 1)
        {
            if (pipe(pipe_fd) < 0)
            {
                perror("pipe");
                return -1;
            }
        }
        int output_fd = (i < cmd_count - 1) ? pipe_fd[1] : STDOUT_FILENO;

        pid_t pid = fork();
        if (pid == 0)
        {
            if (input_fd != STDIN_FILENO)
            {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            if (output_fd != STDOUT_FILENO)
            {
                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
            }
            if (i < cmd_count - 1)
                close(pipe_fd[0]);
            if (is_builtin(args))
            {
                exec_builtin(args);
                exit(0);
            }
            else
            {
                execvp(args[0], args);
                fprintf(stderr, "Unknown command: [%s].\n", args[0]);
                exit(127);
            }
        }
        else if (pid > 0)
            pids[valid_commands++] = pid;
        else
        {
            perror("fork");
            return -1;
        }

        if (output_fd != STDOUT_FILENO)
            close(output_fd);
        if (input_fd != STDIN_FILENO)
            close(input_fd);
        input_fd = (i < cmd_count - 1) ? pipe_fd[0] : STDIN_FILENO;
    }
    for (int i = 0; i < valid_commands; i++)
        waitpid(pids[i], &status, 0);
    return 0;
}