// Used for launching the shell and testing

#include "shell.h"

int main() {

    printf("%s\n", strip("   ls -a -l   \n"));

    launch_shell();
    return 0;
}