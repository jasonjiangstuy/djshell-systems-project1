// File for parsing strings and arguments

// includes
#include "includes.h"

// Strips strings of trailing and leading whitespace; takes a string; returns a string
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

// parsing for argument vector; breaks on spaces; adds trailing null for execvp; takes a string; returns an array of strings
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

int open_history(){
  int fd = open("history.txt",O_RDWR);
  lseek(fd,0,SEEK_END);
  return fd;
}

void parse_data(int fd, char * str, int size, int arrSize){
  // printf("\nsize %d arraysize %d  \n", size, arrSize);
  int index = size;
  if (size > 0){
    read(fd,str, size);
    lseek(fd,-size,SEEK_CUR);
    int index = size;
    for (; index < arrSize; index ++){
      // printf("%d\n", index);
      str[index] = '\0';
    }
  }else{
    for (; index < arrSize; index ++){
      // printf("%d\n", index);
      str[index] = '\0';
    }
  }

  // char arr[1];
  // read(fd,arr,sizeof(char));
  // lseek(fd,-1*sizeof(char),SEEK_CUR);
  // printf("\n PARSE POINTER AT: %c\n", arr[0], size);

}

int prevhistory(int fd){
  FILE* f = fdopen(fd, "r");
  char arr[1];
  int size = 0;
  // lseek(fd,-2*sizeof(char),SEEK_CUR);
  // read(fd,arr,sizeof(char));

  while (arr[0] != '\n'){
    // printf("%c%d \t", arr[0], size);
    if (ftell(f) < 2){
      // we cannot find the new line before the start of file
      lseek(fd, 0, SEEK_SET);
      size = nexthistory(fd);
      lseek(fd, 0, SEEK_SET);
      // return the size of the first commands
      return size;
    }

    size++;
    lseek(fd,-2*sizeof(char),SEEK_CUR);
    read(fd,arr,sizeof(char));

  }
  // read(fd,arr,sizeof(char));
  // lseek(fd,-1*sizeof(char),SEEK_CUR);
  // printf("\nPOINTER AT: %c\n", arr[0], size);

  size --;
  // we hit the new line above, dont count that line

  return size;
}
//didnt use it
void prevhistoryWipe(int fd){
  lseek(fd,0, SEEK_END);
  prevhistory(fd);
  FILE* f = fdopen(fd, "r");
  int size = ftell(f);
  printf("%d\n", size);
  printf("%d\n", ftruncate(fd, size));
  printf("%s\n", strerror(errno));


  // lseek(fd,0, SEEK_END);
  // char arr[1];
  // lseek(fd,-1*sizeof(char),SEEK_CUR);
  // read(fd,arr,sizeof(char));
  // printf("%c\n", arr);

}
int nexthistory(int fd){
  char arr[1];
  int size = 0;
  char test[1];
  lseek(fd,-1, SEEK_CUR);
  read(fd,test,sizeof(char));
  // printf("%d\n", test);

  if (test[0] == EOF){
    // reached end of file
    printf("EOF\n");
    lseek(fd,0, SEEK_END);
    // no command exists
    return 0;
  }
  read(fd,arr,sizeof(char));
  // printf("SURVIEVED\n");
  // printf("%s\n", arr);
  while (arr[0] != '\n'){
    if (test[0] == EOF || arr[0] == EOF){
      printf("EOF\n");

      // reached end of file
      lseek(fd,0, SEEK_END);
      // no command exists
      return 0;
    }
    read(fd,arr,sizeof(char));
    // printf("%c\n", fd);
    size++;
  }
  return size;
}
