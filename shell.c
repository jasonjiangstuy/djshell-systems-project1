// Used for main shell functionality
#include <unistd.h>

// FIXES: MOVE TO NEW FILES; PARSE ON ;
#include "shell.h"

// Logs errors and events to errorlog
void log_error(char *message) {
    int file = open("error_log.txt", O_CREAT | O_WRONLY | O_APPEND);
    if (file == -1) {
        printf("Error opening error log: %s\n", strerror(errno));
    }
    int w = write(file, message, strlen(message));
    if (!w) {
        printf("Error writing to file: %s\n", strerror(errno));
    }
    w = write(file, "\n", 1);
}

// signal handler POSSIBLY REMOVE TO SEPARATE FILE
static void sighandler(int sig) {
    if (sig == SIGINT) {
        exit(0);
    }
    if (sig == SIGSEGV) {
        log_error(strerror(errno));
        exit(-1);
    }
}

// basic parsing for argument vector
char ** parse_args(char *line) {
    int i;
    int counter = 2;
    for (i = 0; i < strlen(line); i++) {
        if (line[i] == ' ') {
            counter++;
        }
    }
    char **arr = calloc(counter, sizeof(char *));

    char *tmp;

    counter = 0;


    char *ptr = strchr(line, '\n');
    if (ptr != NULL) {
        *ptr = '\0';
    }

    while ((tmp = strsep(&line, " "))) {
        arr[counter] = stripOneWord(tmp);
        counter++;
    }

    return arr;

}

int execute(char **args, int fd) {
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }
    else if (strcmp(args[0], "cd") == 0) {
        int status = chdir(args[1]);
        if (status) {
            return -1;
        }
        else return 0;
    }
    int f = fork();
    if (f) {
        int status;
        wait(&status);
        // fd_max do i have to redirect back?13428709
        int return_val = WEXITSTATUS(status);
        if (return_val) {
            return -1;
        }
        return 0;
    }
    else {
        if (fd != 1){
          // its not stdout
          dup(1); // clone stdout
          dup2(fd, 1);
          // redirect that file, to stdout
        }
        int status = execvp(args[0], args);
        if (status == -1) {
            return errno;
        }
        return 0;
    }
}

char * stripOneWord(char *line) {

    char *ptr = line;
    char * newline = calloc(strlen(line), 1);
    int nlcounter = 0;
    int i;
    for (i = 0; i < strlen(line); i++) {
        if (!isspace(line[i])) {
          break;
        }
    }
    // save to another char array
    for (; i< strlen(line); i++){
      // disregard everything after white space
      if (isspace(line[i])) {
        break;
      }
      newline[nlcounter] = line[i];
      nlcounter++;
    }


    // // char *end = ptr + strlen(ptr) - 1;
    // for ( ; end != ptr - 1; end--) {
    //     if (!isspace(*end)) {
    //         break;
    //     }
    //     *end = '\0';
    // }
    return newline;
}

// main launch loop
int launch_shell() {

    printf("Launching shell\n");

    signal(SIGSEGV, sighandler);
    signal(SIGINT, sighandler);

    int file = open("history.txt", O_CREAT | O_RDWR | O_APPEND);
    if (file == -1) {
        printf("Error with launching shell\n");
        log_error(strerror(errno));
        exit(0);
    }

    printf("Please separate all arguments with spaces!\n");

    while (1) {

        char *tmp_path = calloc(CHARMAX, sizeof(char));
        getcwd(tmp_path, CHARMAX);
        char *path = strrchr(tmp_path, '/');

        printf("%s djshell $ ", path);
        char *buffer = calloc(CHARMAX, sizeof(char)); // fix sizing?
        fgets(buffer, CHARMAX, stdin);

        write(file, buffer, strlen(buffer));

        char *tmp;
        while ((tmp = strsep(&buffer, ";"))) {
            tmp[strlen(tmp) - 1] = '\0';
            if (isspace(tmp[0])) {
                tmp++;
            }
            char * currentCommand = tmp;
            int counter;
            for (counter = 0; counter < strlen(tmp); counter++){
              if (tmp[counter] == '|'){
                tmp[counter] = '\0';
                printf("%s | \n", currentCommand[0]);
                // found pipe, end of command, send this to stdin
                // char **args = parse_args(currentCommand);
                // int status = execute(args, 0);
                // if (status != 0) {
                //     printf("%s\n", strerror(errno));
                //     log_error(strerror(errno));
                // }

                currentCommand = tmp + counter + 1;
              }else if(tmp[counter] == '>'){
                tmp[counter] = '\0';
                printf("%s > %s\n", currentCommand[0], currentCommand[1]);
                // // found redirect end of command, send this the file name coming up till the end of the line
                // int fd = open(temp[counter + 1], O_CREAT | O_WRONLY, 0777);
                // // redirect things to stdout to fd
                // char **args = parse_args(currentCommand);
                // int status = execute(args, fd);
                // if (status != 0) {
                //     printf("%s\n", strerror(errno));
                //     log_error(strerror(errno));
                // }
                currentCommand = '\0';
                break;
              }
            }
            if (currentCommand[0] != '\0'){
              printf("%s\n", currentCommand[0]);
              // if there is one command left
              // char **args = parse_args(currentCommand);
              // int status = execute(args, fd);
              // if (status != 0) {
              //     printf("%s\n", strerror(errno));
              //     log_error(strerror(errno));
              // }

            }

        }

    }

    return 0;

}
