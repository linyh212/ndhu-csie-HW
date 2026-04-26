#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

#define MAX_LINE 5000
#define MAX_CMD 256
#define MAX_ARGS 128
#define MAX_CMDS 64
#define MAX_NPIPES 128

typedef struct {int read_fd; int write_fd; int remain; int used;} NumberedPipe;
extern NumberedPipe npipes[MAX_NPIPES];
void init_numbered_pipes(void);
int create_numbered_pipe(int n);
int get_input_from_num_pipe(void);
void update_num_pipes(void);

int execute_line(char *line);
int exec_external(char **args, int input_fd, int output_fd);
int is_builtin(char **args);
int exec_builtin(char **args);
int parse_commands(char *line, char *commands[]);
int execute_pipeline(char **commands, int cmd_count);

#endif
