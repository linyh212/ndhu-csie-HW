#include "shell.h"

char *search_path(char *cmd)
{
    char *path = getenv("PATH");
    if (path == NULL)
        return NULL;
    char *path_copy = strdup(path);
    if (path_copy == NULL)
        return NULL;
    char *dir = strtok(path_copy, ":");
    while (dir != NULL)
    {
        char fullpath[MAX_CMD];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, cmd);
        if (access(fullpath, X_OK) == 0)
        {
            free(path_copy);
            return strdup(fullpath);
        }
        dir = strtok(NULL, ":");
    }
    
    free(path_copy);
    return NULL;
}

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
        for (int i = 0; i < MAX_NPIPES; i++)
        {
            if (npipes[i].used)
            {
                if (npipes[i].read_fd != -1)
                    close(npipes[i].read_fd);
                if (npipes[i].write_fd != -1)
                    close(npipes[i].write_fd);
            }
        }
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