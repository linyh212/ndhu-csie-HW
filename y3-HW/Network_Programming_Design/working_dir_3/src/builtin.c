#include "shell.h"

static const char *basename_cmd(char *cmd)
{
    if (cmd == NULL)
        return NULL;
    char *base = strrchr(cmd, '/');
    return base ? base + 1 : cmd;
}

int is_builtin(char **args)
{
    if (args[0] == NULL)
        return 0;
    const char *cmd = basename_cmd(args[0]);
    if (strcmp(cmd, "quit") == 0)
        return -1;
    if (strcmp(cmd, "setenv") == 0)
        return 1;
    if (strcmp(cmd, "printenv") == 0)
        return 1;
    return 0;
}

int exec_builtin(char **args)
{
    const char *cmd = basename_cmd(args[0]);
    if (strcmp(cmd, "quit") == 0)
        exit(0);
    else if (strcmp(cmd, "setenv") == 0)
    {
        if (args[1] == NULL || args[2] == NULL)
            fprintf(stderr, "Usage: setenv [variable] [value]\n");
        else
        {
            if (setenv(args[1], args[2], 1) != 0)
                perror("setenv");
        }
        return 0;
    }
    else if (strcmp(cmd, "printenv") == 0)
    {
        if (args[1] == NULL)
        {
            extern char **environ;
            for (char **env = environ; *env != NULL; env++)
                printf("%s\n", *env);
        }
        else
        {
            char *val = getenv(args[1]);
            if (val != NULL)
                printf("%s\n", val);
        }
        return 0;
    }
    return 0;
}
