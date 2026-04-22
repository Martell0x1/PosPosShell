#include "shell.h"

int parse_args(char* input , char** args){
int i = 0;

if(input == NULL)
    return 0;

input[strcspn(input , "\n")] = 0;

char* token = strtok(input, " ");

while(token != NULL && i < MAX_ARGS - 1){
    args[i++] = token;
    token = strtok(NULL , " ");
}

args[i] = NULL;

return i;

}
