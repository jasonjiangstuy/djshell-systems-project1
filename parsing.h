// Includes file for parsing.c

#ifndef PARSING
#define PARSING "defined"

// Function declarations
// Strips strings of trailing and leading whitespace; takes a string; returns a string
char * strip(char *line);
// parsing for argument vector; breaks on spaces; adds trailing null for execvp; takes a string; returns an array of strings
char ** parse_args(char *line);
//opens command history file; sets position to end
int open_history();
//parsing for previous command in history file
char ** parse_prevhistory(int fd);
//parsing for current/next command in history file
char ** parse_nexthistory(int fd);

#endif
