// Includes file for parsing.c

#ifndef PARSING
#define PARSING "defined"

// Function declarations
//opens command history file; sets position to end; takes null; returns file descriptor
int open_history();
// Strips strings of trailing and leading whitespace; takes a string; returns a string
char * strip(char *line);
// parsing for argument vector; breaks on spaces; adds trailing null for execvp; takes a string; returns an array of strings
char ** parse_args(char *line);
//parsing for previous command in history file; takes file descriptor; returns command string
char ** parse_prevhistory(int fd);
// parsing for current/next command in history file; takes file descriptor; returns command string
char ** parse_nexthistory(int fd);

#endif
