#include "shell.h"

int is_builtin(char **args)
{
    if (args[0] == NULL)
        return 0;
    if (strcmp(args[0], "quit") == 0)
        return -1;
    if (strcmp(args[0], "setenv") == 0)
        return 1;
    if (strcmp(args[0], "printenv") == 0)
        return 1;
    return 0;
}

int exec_builtin(char **args)
{
    if (strcmp(args[0], "quit") == 0)
        return -1;
    else if (strcmp(args[0], "setenv") == 0)
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
    else if (strcmp(args[0], "printenv") == 0)
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