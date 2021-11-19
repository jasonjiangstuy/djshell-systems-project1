// Used for main shell functionality

// FIXES: WRITE ALL ERRORS TO ERROR_LOG; WRITE A SEPARATE ERROR FUNCTION

#include "shell.h"

// signal handler POSSIBLY REMOVE TO SEPARATE FILE
static void sighandler(int sig) {
    if (sig == SIGINT) {
        int file = open("error_log.txt", O_CREAT | O_WRONLY | O_APPEND);
        if (file == -1) {
            printf("Error Opening Error Log: %s\n", strerror(errno));
        }
        char *message = "program exited due to SIGINT\n";
        write(file, message, 29);
        exit(0);
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

    while ((tmp = strsep(&line, " ")) && line) { 
        if (errno != 0) {
            printf("Error with parsing args: %s\n", strerror(errno));
            exit(-1);
        }
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
        char *buffer = calloc(100, sizeof(char)); // fix sizing?
        fgets(buffer, 100, stdin);
        char **args = parse_args(buffer);
        int f = fork();
        if (f) {
            int status;
            wait(&status);
            int return_val = WEXITSTATUS(status);
            continue;
        } 
        else {
            int status = execvp(args[0], args);
            if (status == -1) {
                printf("Error with execvp: %s\n", strerror(errno));
                return errno;
            }   
            return 0;
        }
    }

    return 0;

}