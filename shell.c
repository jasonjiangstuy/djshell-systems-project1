// Used for main shell functionality

// includes
#include "runs.h"
#include "colors.h"
#include "parsing.h"
#include "includes.h"

// initializer arrays for color printing
char *(colors[8]) = {BRED, BGRN, BYEL, BBLU, BMAG, BCYN, BWHT, reset};
char *(colorNames[8]) = {"RED", "GREEN", "YELLOW", "BLUE", "MAGENTA", "CYAN", "WHITE", "NORMAL"};

// struct declarations
struct termios orig_termios;

// holds ease out function, returns delay between each color at specific time
float yvalue(float x) {
    return x*x / (2.0f * (x * x - x) + 1.0f);
}


// function for printing random colors; takes no args; returns random color
int randomizeColor() {
    int randomColor;
    // immediately flush stdout
    setbuf(stdout, NULL);
    // float delay = .1;
  	float t =0;

  	// for (; delay < 4; delay *= 1.25){
  	for (; t < 1; t += .1) {
    	printf("\33[2K\r");
    	randomColor = rand() % 8;
    	printf("%s", colors[randomColor]);
    	printf("Picking Terminal Color: %s", colorNames[randomColor]);
    	// fflush(stdout);
    	sleep(.75 + yvalue(t));
  	}
  	printf("\n");
  	// char x = '\n';
  	setlinebuf(stdout);
  	return randomColor;
}

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
// https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html

// disables Raw Mode and makes stdin buffered; takes and returns nothing
void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

// allows for reading one char at a time; takes and returns nothing
void enableRawMode() {
  	tcgetattr(STDIN_FILENO, &orig_termios);
  	atexit(disableRawMode);
  	struct termios raw = orig_termios;
  	// read in input char by char + dont print out charaters that are inputed
  	// we will handle that separately
  	raw.c_lflag &= ~(ECHO | ICANON);
  	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// prints out djshell prompt
void prompt(char * path) {
	printf("%s djshell $ ", path);
}

// main launch loop; takes no args; returns an int (should always return 0)
int launch_shell() {

    srand( time(NULL) );
    int choiceColor = randomizeColor(); 

    printf("Launching shell\n");

    // signalhandler
    signal(SIGSEGV, sighandler);
    signal(SIGINT, sighandler);

    // uses a history file to track commands for lseeking (TO IMPLEMENT)
    int file = open("history.txt", O_CREAT | O_RDWR | O_APPEND, 0777);
    if (file == -1) {
        printf("Error with launching shell\n");
        log_error(strerror(errno));
        exit(0);
    }

    printf("Please separate all arguments with spaces!\n");
    // open file to get history
    int history = open_history();

    // loops until exit is asked or ^C sent
    while (1) {

        lseek(history, 0, SEEK_END);
        // gets filepath to display

        char *tmp_path = calloc(CHARMAX, sizeof(char));
        getcwd(tmp_path, CHARMAX);
        char *path = strrchr(tmp_path, '/');

        // used for generating colored output
        prompt(path);
        fflush(stdout);
        char *buffer = calloc(CHARMAX, sizeof(char)); // fix sizing?
        char *dummybuffer = calloc(CHARMAX, sizeof(char)); // fix sizing?

        // waits for input from stdin
        // fgets(buffer, CHARMAX, stdin);
        // raw input
        enableRawMode();
        char c;
        char * escape = calloc(2, sizeof(char));
        unsigned int buffer_int = 0;
        int alreadyinputed = 0;
        while(read(STDIN_FILENO, &c, 1) == 1 && c != 10){
// 0 is present, positive numbers is the past
          // 0 if did not used arrow keys, 1 if did
          // FILE* f = fdopen(history, "r");
          // int size = 0;
          // // if (size > ftell(f)){
          // size = ftell(f);
          // // };
          // printf("%d\n", size);
          // int diff = ftell(f) - size;
          // printf("%s\n", read(history, diff(), sizeof(char));
          if (iscntrl(c)) {
            if (c == 127){
              // backspace
              if (buffer_int >0){
                buffer_int--;
                buffer[buffer_int] = '\0';
                // clear current terminal line and reset to front of terminal
                printf("\33[2K\r");
                prompt(path);
                printf("%s", buffer);
                fflush(stdout);
              }
            }else{
              // printf("%d\n", c);
              read(STDIN_FILENO, escape, 2) == 1;
              // printf("ESCAPE charaters%s\n", escape);
              if (!strcmp(escape, "[A")){
                // printf("uparrow pressed\n");
                // uparrow pressed
                alreadyinputed = 1;
                int size = prevhistory(history);
                parse_data(history, buffer, size, strlen(buffer));
                buffer_int = size;
                printf("\33[2K\r");
                prompt(path);
                printf("%s", buffer);
                fflush(stdout);
              }else if(!strcmp(escape, "[B")){
                // down arrow pressed
                nexthistory(history);
                // printf("%c\n", );
                int size = nexthistory(history);
                prevhistory(history);

                parse_data(history, buffer, size, strlen(buffer));
                buffer_int = size;

                printf("\33[2K\r");
                prompt(path);
                printf("%s", buffer);
                fflush(stdout);
              }
            }

          }else{
              printf("%c", c);
              buffer[buffer_int] = c;
              fflush(stdout);
              buffer_int ++;
            }
        }
        printf("\n");
        disableRawMode();
        // end of raw mode
        // if (alreadyinputed){
        //   // we need to rewrite the temp history command
        //   parse_prevhistory(file);
        //   // move the poiner back a line
        // }
        write(file, buffer, strlen(buffer));
        char * hold = "\n";
        write(file, hold, 1);

        char *tmp;
        // runs a loop separating on ;
        while ((tmp = strsep(&buffer, ";"))) {
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
                        printf("| error\n");
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
                    // printf("COMMAND: %s FILE: %s\n", currentCommand, strip(tmp + counter + 1));
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
