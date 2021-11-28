// Used for main shell functionality

// includes
#include "runs.h"
#include "parsing.h"
#include "includes.h"

// Logs errors and events to errorlog; takes error message; returns void
void log_error(char *message) {
    printf("Error: %s\n", message); // prints all errors --> errors don't cause crashing
    int file = open("error_log.txt", O_CREAT | O_WRONLY | O_APPEND); // opens an error_log
    if (file == -1) {
        printf("Error opening error log: %s\n", strerror(errno));
    }
    int w = write(file, message, strlen(message));
    if (!w) {
        printf("Error writing to file: %s\n", strerror(errno));
    }
    w = write(file, "\n", 1); // adds newline character after 
}

// signal handler; takes int signal; no return, always exits
static void sighandler(int sig) {
    if (sig == SIGINT) {
        printf("\nExiting Shell\n"); // exits shell gracefully on ^C
        exit(0);
    }
    if (sig == SIGSEGV) {
        log_error(strerror(errno)); // won't crash on segfaults // exits gracefully
        exit(-1);
    }
}

// main launch loop; takes no args; returns an int (should always return 0)
int launch_shell() {

    printf("Launching shell\n");

    // signalhandler
    signal(SIGSEGV, sighandler);
    signal(SIGINT, sighandler);

    // uses a history file to track commands for lseeking (TO IMPLEMENT)
    int file = open("history.txt", O_CREAT | O_RDWR | O_APPEND);
    if (file == -1) {
        printf("Error with launching shell\n");
        log_error(strerror(errno));
        exit(0);
    }

    printf("Please separate all arguments with spaces!\n");

    // loops until exit is asked or ^C sent
    while (1) {

        // gets filepath to display
        char *tmp_path = calloc(CHARMAX, sizeof(char));
        getcwd(tmp_path, CHARMAX);
        char *path = strrchr(tmp_path, '/');

        printf("%s djshell $ ", path);
        char *buffer = calloc(CHARMAX, sizeof(char)); // fix sizing?

        // waits for input from stdin
        fgets(buffer, CHARMAX, stdin);

        write(file, buffer, strlen(buffer));

        char *tmp;
        // runs a loop separating on ;
        while ((tmp = strsep(&buffer, ";"))) {
            // null padding for safety
            tmp[strlen(tmp) - 1] = '\0';
            if (isspace(tmp[0])) {
                tmp++;
            }
            char * currentCommand = tmp;
            int counter;
            // loops over command to find |, <, > (>>)
            for (counter = 0; counter < strlen(tmp); counter++) {
                if (tmp[counter] == '|') {
                    // sets to null to split currentcommand into only first half
                    tmp[counter] = '\0';
                    // tmp + counter + 1 is the character after the null
                    int status = execute_pipe(currentCommand, strip(tmp+counter+1));
                    if (status) {
                        log_error(strerror(errno));
                    }
                    // sets currentcommand[0] to null so there is nothing left in the string to parse
                    *currentCommand = '\0';
                }
                // for n>
                else if (isdigit(tmp[counter]) && tmp[counter+1] == '>') {
                    int x = atoi(&tmp[counter]);
                    tmp[counter] = '\0';
                    int fd;
                    if (tmp[counter + 2] == '>') {
                        fd = open(strip(tmp + counter + 3), O_CREAT | O_WRONLY | O_APPEND, 0777);
                    }
                    else {
                        fd = open(strip(tmp + counter + 2), O_CREAT | O_WRONLY | O_TRUNC, 0777);
                    }
                    run(currentCommand, fd, x);
                    *currentCommand = '\0';
                }
                // for &>: redirecting stdout and stderror
                else if (tmp[counter] == '&' && tmp[counter+1] == '>') {
                    tmp[counter] = '\0';
                    int fd;
                    if (tmp[counter + 2] == '>') {
                        fd = open(strip(tmp + counter + 3), O_CREAT | O_WRONLY | O_APPEND, 0777);
                    }
                    else {
                        fd = open(strip(tmp + counter + 2), O_CREAT | O_WRONLY | O_TRUNC, 0777);
                    }
                    run(currentCommand, fd, -2);
                    *currentCommand = '\0';
                }
                else if (tmp[counter] == '>') {
                    tmp[counter] = '\0';
                    int fd;
                    int x = STDOUT_FILENO;
                    // checks for >n
                    if (isdigit(tmp[counter + 1])) {
                        x = atoi(&tmp[counter + 1]);
                        fd = open(strip(tmp + counter + 2), O_CREAT | O_WRONLY | O_TRUNC, 0777);
                    }
                    // checks for >&
                    else if (tmp[counter + 1] == '&') {
                        x = -2;
                        fd = open(strip(tmp + counter + 2), O_CREAT | O_WRONLY | O_TRUNC, 0777);
                    }
                    // if >>, sets for append
                    else if (tmp[counter + 1] == '>') {
                        // checls for >>n
                        if (isdigit(tmp[counter + 2])) {
                            x = atoi(&tmp[counter + 2]);
                            fd = open(strip(tmp + counter + 3), O_CREAT | O_WRONLY | O_APPEND, 0777);
                        }
                        // checks for >>&
                        else if (tmp[counter + 2] == '&') {
                            x = -2;
                            fd = open(strip(tmp + counter + 3), O_CREAT | O_WRONLY | O_APPEND, 0777);
                        }
                        else {
                            fd = open(strip(tmp + counter + 2), O_CREAT | O_WRONLY | O_APPEND, 0777);
                        }
                    }
                    // else sets to writing (overwrite)
                    else {
                        fd = open(strip(tmp + counter + 1), O_CREAT | O_WRONLY| O_TRUNC, 0777);
                    }
                    run(currentCommand, fd, x);
                    *currentCommand = '\0';
                }
                else if (tmp[counter] == '<') {
                    tmp[counter] = '\0';
                    // sets for read only
                    int fd = open(strip(tmp + counter + 1), O_RDONLY);
                    run(currentCommand, fd, STDIN_FILENO);
                    *currentCommand = '\0';
                }
            }
            // if nothing is detected un the loop and we haven't already executed current command, runs it
            if (currentCommand[0] != '\0') {
                int fd = 1;
                run(currentCommand, fd, -1);
            }
        }
    }
    return 0;
}
