#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  printf("HELLO");
  fflush(stdout);
  sleep(2);
  printf("\33[2K\r");
  fflush(stdout);
  printf("%s\n", "Poof gone");
  return 0;
}
