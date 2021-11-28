// Includes file for parsing.c

#ifndef PARSING
#define PARSING "defined"

// Function declarations
// Strips strings of trailing and leading whitespace; takes a string; returns a string
char * strip(char *line);
// parsing for argument vector; breaks on spaces; adds trailing null for execvp; takes a string; returns an array of strings
char ** parse_args(char *line);

#endif