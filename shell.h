// main shell functionality headers

#ifndef SHELL
#define SHELL "defined"

// std includes
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

// Compiler variables
#define CHARMAX 100

// custom functions
int launch_shell();
void log_error(char *message);
static void sighandler(int sig);
char **parse_args( char *line );

#endif