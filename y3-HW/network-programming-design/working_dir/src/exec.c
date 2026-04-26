#include "shell.h"

int exec_external(char **args, int input_fd, int output_fd)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return -1;
    }
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
        execvp(args[0], args);
        fprintf(stderr, "Unknown command: [%s].\n", args[0]);
        exit(127);
    }
    else
    {
        if (input_fd != STDIN_FILENO)
            close(input_fd);
        if (output_fd != STDOUT_FILENO)
            close(output_fd);
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
}