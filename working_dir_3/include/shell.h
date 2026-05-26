#ifndef SHELL_H
#define SHELL_H

/* ================ HW1 ================ */
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

/* ================ HW2 ================ */
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 12345
#define SOCKET_PATH "/tmp/chatd.sock"
#define BUFFER_SIZE 4096
#define MAX_NAME 32
#define MAX_MSG 1024
#define MAX_USER 64

typedef struct {int id; char name[MAX_NAME]; int tcp_fd; int msg_fd;} User;

typedef struct {int sender_id; int cmd; char arg1[256]; char arg2[1024]; char arg3[256];} Request;

#define CMD_WHO 1
#define CMD_TELL 2
#define CMD_YELL 3
#define CMD_NAME 4

/* ================ HW3 ================ */
#define CMD_LOGIN       10
#define CMD_REGISTER    11
#define CMD_LISTMAIL    12
#define CMD_MAILTO      13
#define CMD_DELMAIL     14
#define CMD_CREATEGROUP 15
#define CMD_DELGROUP    16
#define CMD_ADDTO       17
#define CMD_LEAVEGROUP  18
#define CMD_LISTGROUP   19
#define CMD_REMOVE      20
#define CMD_GYELL       21
#define CMD_LOGOUT      22

/* ================ HW1 ================ */
void init_numbered_pipes(void);
int create_numbered_pipe(int n);
int get_input_from_num_pipe(void);
void update_num_pipes(void);
void close_all_numbered_pipes(void);
int execute_line(char *line);
int exec_external(char **args, int input_fd, int output_fd);
char *search_path(char *cmd);
int is_builtin(char **args);
int exec_builtin(char **args);
int parse_commands(char *line, char *commands[]);
int parse_args(char *cmd_str, char *args[]);
int execute_pipeline(char **commands, int cmd_count, int input_fd, int numbered_output_fd);
void cleanup_zombies(void);

/* ================ HW2 ================ */
int allocate_id(void);
User* find_user_by_id(int id);
User* find_user_by_name(const char *name);
void add_user(int tcp_fd, int msg_fd, int id, const char *name);
void remove_user(int tcp_fd);
void send_to_user(int target_id, const char *msg);
void yell_to_all(const char *msg, int sender_id);
void handle_local_request(int client_fd);
void start_local_server(void);
void sigchld_handler(int sig);

#endif