#include "shell.h"

NumberedPipe npipes[MAX_NPIPES];

void init_numbered_pipes(void) {
    for (int i = 0; i < MAX_NPIPES; i++) {
        npipes[i].used = 0;
        npipes[i].read_fd = -1;
        npipes[i].write_fd = -1;
        npipes[i].remain = 0;
    }
}

int create_numbered_pipe(int n) {
    for (int i = 0; i < MAX_NPIPES; i++) {
        if (!npipes[i].used) {
            int fd[2];
            if (pipe(fd) < 0) {
                perror("pipe");
                return -1;
            }
            npipes[i].read_fd = fd[0];
            npipes[i].write_fd = fd[1];
            npipes[i].remain = n;
            npipes[i].used = 1;
            return fd[1];
        }
    }
    fprintf(stderr, "Too many numbered pipes\n");
    return -1;
}

void update_num_pipes(void) {
    for (int i = 0; i < MAX_NPIPES; i++) {
        if (npipes[i].used && npipes[i].remain > 0)
            npipes[i].remain--;
    }
}

int get_input_from_num_pipe(void) {
    for (int i = 0; i < MAX_NPIPES; i++) {
        if (npipes[i].used && npipes[i].remain == 0) {
            int fd = npipes[i].read_fd;
            npipes[i].used = 0;
            return fd;
        }
    }
    return -1;
}