// main shell functionality headers

#ifndef SHELL
#define SHELL "defined"

// custom functions
// main launch loop; takes no args; returns an int (should always return 0)
int launch_shell();
// function for printing random colors; takes no args; returns random color
int randomizeColor();
// allows for reading one char at a time; takes and returns nothing
void enableRawMode();
// holds ease out function, returns delay between each color at specific time
float yvalue(float x);
// disables Raw Mode and makes stdin buffered; takes and returns nothing
void disableRawMode();
// prints out djshell prompt
void prompt(char * path);
// Logs errors and events to errorlog; takes error message; returns void
void log_error(char *message); 
// signal handler; takes int signal; no return, always exits
static void sighandler(int sig);

#endif
