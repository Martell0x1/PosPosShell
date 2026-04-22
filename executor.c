#include "shell.h"

void execute_command(char *input) {
  char* args[MAX_ARGS];

  char input_cpy[MAX_INPUT];
  strcpy(input_cpy,input);
  
  int arg_count = parse_args(input,args);
  
  if(arg_count == 0){
    return;
  }

  // cd , pwd , ls
  
  if(handle_commands(arg_count,args))
    return ;

  int background = 0;
  if(strcmp(args[arg_count -1],"&") == 0)
  {
    background = 1;
    args[arg_count -1] = NULL;
  }

  execute_a_fork_family(args,background);
}

void execute_a_fork_family(char ** args , int background){
  pid_t pid = fork();

  if(pid == 0){
    // Handle redirections
    int i = 0;
    while (args[i] != NULL) {
      if (strcmp(args[i], ">") == 0) {
        if (args[i+1] != NULL) {
          int fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
          if (fd == -1) {
            perror("open output");
            exit(EXIT_FAILURE);
          }
          dup2(fd, STDOUT_FILENO);
          close(fd);
          // Remove > and filename from args
          int j = i;
          while (args[j+2] != NULL) {
            args[j] = args[j+2];
            j++;
          }
          args[j] = NULL;
          continue;
        } else {
          fprintf(stderr, "Syntax error: no file provided\n");
          exit(EXIT_FAILURE);
        }
      } else if (strcmp(args[i], "<") == 0) {
        if (args[i+1] != NULL) {
          int fd = open(args[i+1], O_RDONLY);
          if (fd == -1) {
            perror("open input");
            exit(EXIT_FAILURE);
          }
          dup2(fd, STDIN_FILENO);
          close(fd);
          // Remove < and filename from args
          int j = i;
          while (args[j+2] != NULL) {
            args[j] = args[j+2];
            j++;
          }
          args[j] = NULL;
          continue;
        } else {
          fprintf(stderr, "Syntax error: no file provided\n");
          exit(EXIT_FAILURE);
        }
      } else {
        i++;
      }
    }

    execvp(args[0],args);
    perror("Command Not Found");
    exit(EXIT_FAILURE);

  }

  else if (pid > 0 ){
    if(!background){
      waitpid(pid,NULL,0);
    }
    else {
      printf("Background PID: %d\n",pid);
    }
  }
  else {
    perror("Fork Failed");
  }
}
