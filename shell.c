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
        printf("\nExiting Shell\n");
        exit(0);
    }
    if (sig == SIGSEGV) {
        log_error(strerror(errno));
        exit(-1);
    }
}

// basic parsing for argument vector
char ** parse_args(char *line) {
    line = strip(line);
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
        arr[counter] = strip(tmp);

        // printf("arr[counter] %s\n", arr[counter]);
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

int execute_pipe(char *src) {
    printf("%s ready to recieve\n", src);
    FILE * in = popen(src, "w");
    dup2(fileno(in), STDOUT_FILENO);
    return 0;
}

// input make it calloc
char * strip(char *line) {
    // printf("%s\n", line);
    char * ptr = calloc(strlen(line), sizeof(char));
    strcpy(ptr, line);
    for (; ptr[0]!= '\0'; ptr++) {
        if (!isspace(ptr[0])) {
          break;
        }else{
          ptr[0] = '\0';
        }
    }
    int i;
    for (i = strlen(ptr) - 1; i >= 0; i --){
      if (!isspace(ptr[i])) {
        break;
      }else{
        ptr[i] = '\0';
      }
    }
    return ptr;
}

int run(char * currentCommand){
    printf("running %s\n", currentCommand);
    char **args = parse_args(currentCommand);
    int status = execute(args);
    if (status != 0) {
        log_error(strerror(errno));
    }
    return 0;
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
        fgets(buffer + 1, CHARMAX -1, stdin);
// + 1 to leave a null at the start of char array
        write(file, buffer, strlen(buffer));

        char *tmp;
        while ((tmp = strsep(&buffer, ";"))) {
            tmp = tmp +1;
            tmp[strlen(tmp) - 1] = '\0';
            int backupOUT = dup(STDOUT_FILENO);
            int backupIN = dup(STDIN_FILENO);
            // tmp is a single Command
            tmp = strip(tmp);
            int size = strlen(tmp);
            if (size == 0){
              break;
            }
            char * currentCommand;
            // iterate backwards
            int counter = 0;
            int go = 1;
            for (; go; counter ++){
              int currentIndex = size - 1 - counter;
              switch (tmp[currentIndex]) {
                case '|':
                  currentCommand = tmp + currentIndex + 1;
                  tmp[currentIndex] = '\0';
                  execute_pipe(strip(currentCommand));
                  break;
                case '>':
                  tmp[currentIndex] = '\0';
                  if (tmp[currentIndex - 1] == '>') {
                      counter ++;
                      tmp[currentIndex - 1] = '\0';
                      currentCommand = tmp+ currentIndex + 1;
                      int fd = open(strip(currentCommand), O_CREAT | O_WRONLY | O_APPEND, 0777);
                      dup2(fd, STDOUT_FILENO);
                  }
                  else {
                    // redirect to file, overwriting
                      currentCommand = tmp + currentIndex + 1;
                      int fd = open(strip(currentCommand), O_CREAT | O_WRONLY| O_TRUNC, 0777);
                      dup2(fd, STDOUT_FILENO);
                  }
                  break;
                case '<':
                // taking in a file for input
                  tmp[currentIndex] = '\0';
                  // now tmp is the file name
                  currentCommand = tmp + currentIndex + 1;
                  int fd = open(strip(tmp), O_RDONLY);
                  execute_rein(currentCommand, fd);
                  // stop the program;
                  counter ++;
                  break;
                case '\0':
                  // reached end of command
                  currentCommand = tmp + currentIndex + 1;
                  run(currentCommand);
                  go = 0;
                  break;
              }
              printf("%s\n", "end");
            }
            dup2(backupOUT, STDOUT_FILENO);
            dup2(backupIN, STDIN_FILENO);

        }
    }
    return 0;
}
