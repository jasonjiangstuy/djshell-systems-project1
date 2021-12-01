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
//opens command history file; sets position to end
int open_history();
void parse_data(int fd, char * str, int size, int arrSize);
void prevhistoryWipe(int fd);
int prevhistory(int fd);
int nexthistory(int fd);


#endif
