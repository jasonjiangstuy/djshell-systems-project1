// Includes for runs.c

#ifndef RUNS
#define RUNS "defined"

// Function declarations
int execute(char **args, int fd, int dest); // executes all commands except for piping
int execute_pipe(char *src, char *dest); // executes piping commands
int run(char *currentCommand, int fd, int dest); // driver for executing commands; parses/preprocesses

#endif