#include "shell.h"

int parse_args(char* input , char** args){
  int i = 0;
  input[strcspn(input , "\n")] = 0;

  char* token =  strtok(input, " ");

  while(token != NULL){
    args[i++] = token;
    token = strtok(NULL , " ");
  }
  args[i] = NULL;

  return i;
}

// ls -l -a 
// 0  1   2 3
