#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include <ctype.h>

int main(int argc, char const *argv[]) {
// HOW TO PIPE
  // get the input file descripter for wc
  // forks child process that is waiting for its input
  // sets output to the output of the parent (stdout)
  FILE * in = popen("wc", "w");
    // set the input file descripter to be file descripter 1
  // MAKE LS OUTPUT TO WC's INPUT
  dup(1); 
  dup2(fileno(in), 1);

  // current file table
  // 0 stdin
  // 1 wc
  // 2 stderr
  // ?? wc
  // ?? stout

  // RUN LS
  int f = fork();
  if (f) {
      int status;
      wait(&status);
      int return_val = WEXITSTATUS(status);
      if (return_val) {
          return -1;
      }
      return 0;
  }
  else {
      char *args[3] = {"ls"};
      int status = execvp(args[0], args);
      if (status == -1) {
          return errno;
      }
      // dup2(backup, STDIN_FILENO);
      return 0;
  }
  // reorganize file table

  return 0;
}
