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

int execute_pipeline(char **commands, int cmd_count, int input_fd, int numbered_output_fd)
{
    int pipe_fd[2];
    int current_input_fd = input_fd;
    pid_t pids[MAX_CMDS];
    int pid_count = 0;
    int status;
    for (int i = 0; i < cmd_count; i++)
    {
        char cmd_copy[MAX_CMD];
        strcpy(cmd_copy, commands[i]);
        char *args[MAX_ARGS];
        parse_args(cmd_copy, args);
        if (args[0] == NULL)
        {
            fprintf(stderr, "Invalid command\n");
            return 0;
        }

        int builtin_type = is_builtin(args);
        if (builtin_type == 2)
        {
            fprintf(stderr, "Unknown command: [%s].\n", args[0]);
            return 0;
        }
        if (builtin_type == 0)
        {
            char *fullpath = search_path(args[0]);
            if (fullpath == NULL)
            {
                fprintf(stderr, "Unknown command: [%s].\n", args[0]);
                return 0;
            }
            free(fullpath);
        }
    }
    for (int i = 0; i < cmd_count; i++)
    {
        char cmd_copy[MAX_CMD];
        strcpy(cmd_copy, commands[i]);
        char *args[MAX_ARGS];
        parse_args(cmd_copy, args);
        if (args[0] == NULL)
        {
            fprintf(stderr, "Invalid command\n");
            continue;
        }
        int output_fd;
        int need_pipe = 0;
        if (i < cmd_count - 1)
        {
            if (pipe(pipe_fd) < 0)
            {
                perror("pipe");
                return -1;
            }
            output_fd = pipe_fd[1];
            need_pipe = 1;
        }
        else
            output_fd = (numbered_output_fd != -1) ? numbered_output_fd : STDOUT_FILENO;
        int builtin_type = is_builtin(args);
        pid_t pid = fork();
        if (pid == 0)
        {
            if (current_input_fd != STDIN_FILENO)
            {
                dup2(current_input_fd, STDIN_FILENO);
                close(current_input_fd);
            }
            if (output_fd != STDOUT_FILENO)
            {
                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
            }
            if (need_pipe)
                close(pipe_fd[0]);
            for (int j = 0; j < MAX_NPIPES; j++)
            {
                if (npipes[j].used)
                {
                    if (npipes[j].read_fd != -1)
                        close(npipes[j].read_fd);
                    if (npipes[j].write_fd != -1)
                        close(npipes[j].write_fd);
                }
            }

            if (builtin_type == 1)
            {
                exec_builtin(args);
                exit(0);
            }
            else
            {
                char *fullpath = search_path(args[0]);
                if (fullpath == NULL)
                {
                    fprintf(stderr, "Unknown command: [%s].\n", args[0]);
                    exit(127);
                }
                execv(fullpath, args);
                perror("execv");
                free(fullpath);
                exit(126);
            }
        }
        else if (pid > 0)
            pids[pid_count++] = pid;
        else
        {
            perror("fork");
            return -1;
        }
        if (current_input_fd != STDIN_FILENO)
            close(current_input_fd);
        if (need_pipe)
        {
            close(pipe_fd[1]);
            current_input_fd = pipe_fd[0];
        }
        else
        {
            if (output_fd != STDOUT_FILENO && output_fd != numbered_output_fd)
                close(output_fd);
            current_input_fd = STDIN_FILENO;
        }
    }
    for (int i = 0; i < pid_count; i++)
        waitpid(pids[i], &status, 0);
    return 0;
}