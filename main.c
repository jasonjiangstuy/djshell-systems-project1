// Used for launching the shell

// includes
#include "shell.h"
#include <stdlib.h>
#include <stdio.h>
#include "parsing.h"
// takes no args; returns 0 always; launches shell
int main() {
    // char * str = calloc(100, sizeof(char));
    // int arrSize = 100;
    // int history = open_history();
    // prevhistoryWipe(history);
    // int size = prevhistory(history);
    // // parse_data(history, str, size, arrSize);
    //
    // size = prevhistory(history);
    // parse_data(history, str, size, arrSize);
    // printf("first % d\n", size);
    // printf("%s\n", str);
    //
    // size = prevhistory(history);
    // parse_data(history, str, size, arrSize);
    //
    // printf("%d\n", size);
    // printf("%s\n", str);

    launch_shell();
    return 0;
}
