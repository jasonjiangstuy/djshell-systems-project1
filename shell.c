// Used for main shell functionality

// FIXES: WRITE ALL ERRORS TO ERROR_LOG; WRITE A SEPARATE ERROR FUNCTION

#include "shell.h"

// Logs errors and events to errorlog
void log_error(char *message) {
    int file = open("error_log.txt", O_CREAT | O_WRONLY | O_APPEND);
    if (file == -1) {
        printf("Error opening error log: %s\n", strerror(errno));
        exit(-1);
    }
    int w = write(file, message, strlen(message));
    if (!w) {
        printf("Error writing to file: %s\n", strerror(errno));
        exit(-1);
    }
    exit(0);
}

// signal handler POSSIBLY REMOVE TO SEPARATE FILE
static void sighandler(int sig) {
    if (sig == SIGINT) {
        log_error("program exited due to SIGINT\n");
    }
}

// basic parsing for argument vector
char ** parse_args(char *line) {

    int i;
    int counter = 1;
    for (i = 0; i < strlen(line); i++) {
        if (line[i] == ' ') {
            counter++;
        }
    }
    char **arr = calloc(counter, sizeof(char *));

    char *tmp;

    counter = 0;

    char *ptr = strchr(line, '\n');
    *ptr = '\0';

    while ((tmp = strsep(&line, " "))) { 
        arr[counter] = tmp;
        counter++;
    }
    
    return arr;

}   

// main launch loop
// add fork
int launch_shell() {

    signal(SIGINT, sighandler);

    printf("Launching shell\n");
    while (1) {
        printf("djshell$ ");
        char *buffer = calloc(CHARMAX, sizeof(char)); // fix sizing?
        fgets(buffer, CHARMAX, stdin);
        char **args = parse_args(buffer);
        int f = fork();
        if (f) {
            int status;
            wait(&status);
            int return_val = WEXITSTATUS(status);
            if (!return_val) {
                log_error("Error with execvp\n");
            }
            continue;
        } 
        else {
            int status = execvp(args[0], args);
            if (status == -1) {
                return errno;
            }   
            return 0;
        }
    }

    return 0;

}