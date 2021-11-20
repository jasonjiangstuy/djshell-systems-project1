// main shell functionality headers

#ifndef MY_SHELL
#define MY_SHELL "defined"

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

// custom functions
int launch_shell();
static void sighandler();
char **parse_args( char *line );

#endif