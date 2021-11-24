// main shell functionality headers

#ifndef SHELL
#define SHELL "defined"

// Compiler variables
#define CHARMAX 100

// Std includes
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include <ctype.h>

// custom functions
int launch_shell();
char * stripOneWord(char *line);
int execute(char **args, int fd);
void log_error(char *message);
static void sighandler(int sig);
char **parse_args( char *line );

#endif
