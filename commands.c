#include "shell.h"

int handle_commands(int arg_count, char** args){

if(arg_count == 0 || args[0] == NULL)
    return 1;

if(strcmp(args[0], "exit") == 0){
    exit(0);
}

if(strcmp(args[0], "cd") == 0){
    if(arg_count < 2){
        fprintf(stderr, "cd: missing argument\n");
        return 1;
    }

    if(chdir(args[1]) != 0){
        perror("cd failed");
    }
    return 1;
}

if(strcmp(args[0], "pwd") == 0){
    char cwd[1024];
    if(getcwd(cwd, sizeof(cwd)) == NULL){
        perror("pwd failed");
    } else {
        printf("%s\n", cwd);
    }
    return 1;
}

if(strcmp(args[0], "history") == 0){
    print_history();
    return 1;
}

return 0;

}
