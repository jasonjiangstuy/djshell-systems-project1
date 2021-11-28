// File for parsing strings and arguments

// includes
#include "includes.h"

// Strips strings of trailing and leading whitespace
char * strip(char *line) {

    // creates a modifiable copy of line
    char * ptr = calloc(strlen(line), sizeof(char));
    strcpy(ptr, line);
    // loops over ptr; keeps incrementing the address of pointer until it's not whitespace
    for ( ; ptr[0] != '\0'; ptr++) {
        if (!isspace(ptr[0])) {
          break;
        } 
        else {
            ptr[0] = '\0';
        }
    }
    int i;
    // goes backwards through pointer, setting whitespace to null until hits character
    for (i = strlen(ptr) - 1; i >= 0; i --) {
        if (!isspace(ptr[i])) {
            break;
        } 
        else {
            ptr[i] = '\0';
        }
    }
    
    return ptr;
}

// parsing for argument vector; breaks on spaces; adds trailing null for execvp
char ** parse_args(char *line) {

    line = strip(line);
    int i;
    int counter = 2; // ls -a -l has two spaces, but needs three elements and a null
    for (i = 0; i < strlen(line); i++) {
        if (line[i] == ' ') {
            counter++; // determines number of spaces to determine length
        }
    }
    char **arr = calloc(counter, sizeof(char *));

    char *tmp;
    counter = 0;

    // technical necessity --> makes sure strsep ends at the right place
    char *ptr = strchr(line, '\n');
    if (ptr != NULL) {
        *ptr = '\0';
    }

    while ((tmp = strsep(&line, " "))) {
        arr[counter] = strip(tmp); // fills array with elements
        counter++;
    }

    return arr;
}