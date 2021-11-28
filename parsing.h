// Includes file for parsing.c

#ifndef PARSING
#define PARSING "defined"

// Function declarations
char * strip(char *line); // Strips strings of trailing and leading whitespace
char ** parse_args(char *line); // parsing for argument vector; breaks on spaces; adds trailing null for execvp

#endif