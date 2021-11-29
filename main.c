// Used for launching the shell

// includes
#include "shell.h"
#include <ncurses.h>
// takes no args; returns 0 always; launches shell
int main() {
  // enter curses mode
    initscr();
    int ch;
    ch = getch();
    printw("%d\n", ch);
    refresh();

    launch_shell();
    return 0;
}
