// File for executing commands

// includes
#include "shell.h"
#include "parsing.h"
#include "includes.h"

// executes all commands except for piping; takes an array of strings, the file descriptor to reidrect to, and file descriptor of either stdin or stdout; returns an int status
int execute(char **args, int fd, int dest) {
    if (strcmp(args[0], "exit") == 0) { // if exit, exit
        exit(0);
    }
    else if (strcmp(args[0], "cd") == 0) { // if cd, use chdir
        int status = chdir(args[1]);
        if (status) {
            return -1;
        }
        else return 0;
    }

    int f = fork(); // forks bcs execvp auto-exits
    if (f) {
        int status;
        wait(&status); // parent waits for return value
        int return_val = WEXITSTATUS(status);
        if (return_val) {
            return -1;
        }
        return 0;
    }
    else {
        if (fd != -1) { // if using redirection, redirects output
            if (dest == -2) { // -2 for dest signifies &>
                // dup(STDERR_FILENO);
                dup(STDOUT_FILENO);
                // dup2(fd, STDERR_FILENO);
                dup2(fd, STDOUT_FILENO);
            }
            else {
                dup(dest);
                dup2(fd, dest);
            }
        }
        int status = execvp(args[0], args);
        if (status == -1) {
            log_error(strerror(errno));
            printf("testchild:\n");

            exit(0);
        }

        return 0;
    }

}

// executes piping commands; takes 2 args, second to pipe first output into; returns int status
int execute_pipe(char *src, char *dest) { // src is first arg to run, dest is where to pipe
    // opens a filestream for the dest using popen
    FILE *in = popen(dest, "w");
    int backup = dup(STDOUT_FILENO); // redirecting stdout to be stdin for in to redirect output of one command to second
    int f = fork(); // forks as usual
    if (f) {
        int status;
        wait(&status);
        int return_val = WEXITSTATUS(status);
        dup2(backup, STDOUT_FILENO);
        if (pclose(in) == -1) { // pclose to make sure in has closed
            return -1;
        }
        if (return_val) {
            return -1;
        }
        return 0;
    }
    else {
        // redirects and executes src
        dup2(fileno(in), STDOUT_FILENO);
        char **args = parse_args(src);
        int status = execvp(args[0], args);
        if (status == 1) {
            return errno;
        }
        return 0;
    }
}

// driver for executing commands; parses/preprocesses; takes string line from shell, file descriptor to redirect to, and whether to use stdin or stdout; returns int status
int run(char *currentCommand, int fd, int dest) {
    char **args = parse_args(currentCommand); // parses args
    int status = execute(args, fd, dest); // executes
    if (status != 0) {
        log_error(strerror(errno)); // logs error
    }
    return 0;
}
