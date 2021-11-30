# djshell-systems-project1

## Implemented Features
- Piping: Shell can pipe an indefinite number of commands
- Redirection: >, >>, and < all work
    - "command n>(>) file" will redirect file descriptor n to file
    - "command &>(>) file" will redirect both stdout and stderror to file
    - Order doesn't matter: >(>)n, >(>)& all work, too
    - Please only use one of the three per prompt
    - < should only be used on its own
- Allows combining indefinitely many pipes followed by > or >>
- Error logging: all errors are recorded in error_log.txt
- History logging: all commands are logged in history.txt
- Random color prompt: Terminal sets to a different colors at start
    - Credit to GitHub user RabaDabaDoba for providing the .h file

## Attempted Features
- Using ncurse to deal with raw input: soon realized that it's overkill
- Accessing speakers to play "remixed" music generated through user input (commands), but apparently accessing speakers to play sound is very difficult in C and variable depending on OS

## Bugs / Things to Know
- Command prompt will take a maximum of 100 characters
- We kindly ask all commands and symbols are separated by spaces; the shell may work anyways, but we programmed with spaces assumed so 'tis best to avoid unforeseen errors and use spaces
- On Mac devices only, using echo on 11-letter words will return odd messages
    - Ex: echo NORMAL TEST returns NORMAL TEST�@P�
    - Sometimes, this also crashes the terminal
    - Error is not present on other OS, including school machines
- Using < assumes everything after < is a filename
    - echo hello < test.txt | wc will treat "test.txt | wc" as a name

## Citations
 - https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html
    - Used their code to read in raw input

## Function Headers
```
int open_history();
int launch_shell();
int randomizeColor();
void enableRawMode();
float yvalue(float x);
void disableRawMode();
void prompt(char * path);
char * strip(char *line);
void log_error(char *message);
char ** parse_args(char *line);
static void sighandler(int sig);
char ** parse_prevhistory(int fd);
char ** parse_nexthistory(int fd);
int execute_pipe(char *src, char *dest);
int execute(char **args, int fd, int dest);
int run(char *currentCommand, int fd, int dest);
```
