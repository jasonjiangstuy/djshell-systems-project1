// Used for main shell functionality

#include "shell.h"

// main launch loop
int launch_shell() {

    printf("Launching shell\n");
    while (1) {
       printf("djshell$ ");
       char buffer[100];
       fgets(stdin, buffer, 100);
    }

}