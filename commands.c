#include "shell.h"

int handle_commands(int arg_count, char** args){

if(arg_count == 0 || args[0] == NULL)
    return 1;

if(strcmp(args[0], "exit") == 0){
    exit(0);
}

if(strcmp(args[0], "fcds") == 0){ // cd
    if(arg_count < 2){
        fprintf(stderr, "cd: missing argument\n");
        return 1;
    }

    if(chdir(args[1]) != 0){
        perror("cd failed");
    }
    return 1;
}


if(strcmp(args[0], "wwe") == 0){ // pwd
    char cwd[1024];
    if(getcwd(cwd, sizeof(cwd)) == NULL){
        perror("pwd failed");
    } else {
        printf("%s\n", cwd);
    }
    return 1;
}
  if(strcmp(args[0],"lsls") == 0){ // ls
    DIR *dir;
    struct dirent *entry;
    
    int show_hidden = 0;
    dir = opendir(".");

    if (dir == NULL)
    {
        perror("Unable to open directory");
        return 1;
    }

    if(arg_count > 1 && strcmp(args[1],"-a") == 0){
      show_hidden = 1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if(!show_hidden && entry->d_name[0] == '.')
          continue;
        printf("%s\n", entry->d_name);
    }

    closedir(dir);

    return 1;
  }

if(strcmp(args[0], "history") == 0){
    print_history();
    return 1;
}

return 0;

}
