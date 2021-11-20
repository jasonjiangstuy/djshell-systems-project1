// Used for main shell functionality

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

int execute(char **args) {
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
        int return_val = WEXITSTATUS(status);
        if (return_val) {
            return -1;
        }
        return 0;
    }
    else {
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

    signal(SIGSEGV, sighandler);
    signal(SIGINT, sighandler);

    printf("Launching shell\n");
    while (1) {

        char *tmp_path = calloc(CHARMAX, sizeof(char));
        getcwd(tmp_path, CHARMAX);
        char *path = strrchr(tmp_path, '/');

        printf("%s djshell $ ", path);
        char *buffer = calloc(CHARMAX, sizeof(char)); // fix sizing?
        fgets(buffer, CHARMAX, stdin);

        char *tmp;
        while ((tmp = strsep(&buffer, ";"))) {
            tmp[strlen(tmp) - 1] = '\0';
            if (isspace(tmp[0])) {
                tmp++;
            } 
            char **args = parse_args(tmp);
            int status = execute(args);
            if (status != 0) {
                printf("%s\n", strerror(errno));
                log_error(strerror(errno));
            }
        }

    }

    return 0;

}
