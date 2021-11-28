// main shell functionality headers

#ifndef SHELL
#define SHELL "defined"

// custom functions
// main launch loop; takes no args; returns an int (should always return 0)
int launch_shell();
// Logs errors and events to errorlog; takes error message; returns void
void log_error(char *message); 
// signal handler; takes int signal; no return, always exits
static void sighandler(int sig);

#endif
