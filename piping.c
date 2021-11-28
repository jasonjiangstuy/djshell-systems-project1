#include "shell.h"

int execute_rein(char * currentCommand, int fd) {
    char **args = parse_args(currentCommand);
    int f = fork();
    if (f) {
        int status;
        wait(&status);
        int return_val = WEXITSTATUS(status);
        if (return_val) {
            return -1;
        }
        return 0;
    }
    else {
        int backup = dup(STDIN_FILENO);
        dup2(fd, STDIN_FILENO);
        int status = execvp(args[0], args);
        if (status == -1) {
            return errno;
        }
        dup2(backup, STDIN_FILENO);
        return 0;
    }
}

int execute_pipe(char *src, char *dest) {
    FILE *in = popen(dest, "w");
    int backup = dup(STDOUT_FILENO);
    int f = fork();
    if (f) {
        int status;
        wait(&status);
        int return_val = WEXITSTATUS(status);
        dup2(backup, STDOUT_FILENO);

        if (pclose(in) == -1) {
            return -1;
        }
        if (return_val) {
            return -1;
        }

        return 0;
    }
    else {
        dup2(fileno(in), STDOUT_FILENO);
        char **args = parse_args(src);
        int status = execvp(args[0], args);
        if (status == 1) {
            return errno;
        }


        return 0;
    }

}
