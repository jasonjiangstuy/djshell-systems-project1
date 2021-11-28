// Includes for runs.c

#ifndef RUNS
#define RUNS "defined"

// Function declarations
// executes piping commands; takes 2 args, second to pipe first output into; returns int status
int execute_pipe(char *src, char *dest); 
// executes all commands except for piping; takes an array of strings, the file descriptor to reidrect to, and file descriptor of either stdin or stdout; returns an int status
int execute(char **args, int fd, int dest);
// driver for executing commands; parses/preprocesses; takes string line from shell, file descriptor to redirect to, and whether to use stdin or stdout; returns int status
int run(char *currentCommand, int fd, int dest); 

#endif