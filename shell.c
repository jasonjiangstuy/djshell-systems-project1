// Used for main shell functionality

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
    printf("%d\n", counter);
    char **arr = calloc(counter, sizeof(char *));

    char *tmp;

    counter = 0;

    while ((tmp = strsep(&line, " "))) {
        if (errno != 0) {
            printf("Error: %s\n", strerror(errno));
            exit(-1);
        }
        arr[counter] = tmp;
        counter++;
    }
    
    return arr;

}   

// main launch loop --> gives error with ls: illegal option -- 
// usage: ls [-@ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx1%] [file ...]
// if ls -a -l is given
int launch_shell() {

    signal(SIGINT, sighandler);

    printf("Launching shell\n");
    while (1) {
        printf("djshell$ ");
        char *buffer = calloc(100, sizeof(char)); // fix sizing?
        fgets(buffer, 100, stdin);
        printf("%s\n", buffer);
        char **args = parse_args(buffer);
        int status = execvp(args[0], args);
        if (status == -1) {
            printf("Error with execvp: %s\n", strerror(errno));
        }
    }

    return 0;

}