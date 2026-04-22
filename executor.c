#include "shell.h"

  // PIPE 

void execute_pipe(char *input) {

    char input_cpy[MAX_INPUT];
    strcpy(input_cpy, input);

    char *pipe_pos = strchr(input_cpy, '|');

    if (pipe_pos == NULL) return;

    // split string
    *pipe_pos = '\0';

    char *cmd1 = input_cpy;
    char *cmd2 = pipe_pos + 1;

    // trim spaces
    while (*cmd2 == ' ') cmd2++;

    char *args1[MAX_ARGS];
    char *args2[MAX_ARGS];

    parse_args(cmd1, args1);
    parse_args(cmd2, args2);

    int fd[2];
    pipe(fd);

    pid_t p1 = fork();

    if (p1 == 0) {
        dup2(fd[1], STDOUT_FILENO);

        close(fd[0]);
        close(fd[1]);

        execvp(args1[0], args1);
        perror("cmd1 failed");
        exit(EXIT_FAILURE);
    }

    pid_t p2 = fork();

    if (p2 == 0) {
        dup2(fd[0], STDIN_FILENO);

        close(fd[1]);
        close(fd[0]);

        execvp(args2[0], args2);
        perror("cmd2 failed");
        exit(EXIT_FAILURE);
    }

    close(fd[0]);
    close(fd[1]);

    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);
}


  // Main Command Execution

void execute_command(char *input) {

  // PIPE Check 
  if (strchr(input, '|') != NULL) {
      execute_pipe(input);
      return;
  }

  char* args[MAX_ARGS];

  char input_cpy[MAX_INPUT];
  strcpy(input_cpy, input);

  int arg_count = parse_args(input, args);

  if(arg_count == 0){
    return;
  }

  // cd , pwd , ls
  if(handle_commands(arg_count, args))
    return;

  // background check
  int background = 0;
  if(strcmp(args[arg_count -1], "&") == 0)
  {
    background = 1;
    args[arg_count -1] = NULL;
  }

  execute_a_fork_family(args, background);
}


   // Execute Normal Commands

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

          args[i] = NULL;
        } else {
          fprintf(stderr, "Syntax error: no file provided\n");
          exit(EXIT_FAILURE);
        }
      }

      else if (strcmp(args[i], "<") == 0) {
        if (args[i+1] != NULL) {
          int fd = open(args[i+1], O_RDONLY);
          if (fd == -1) {
            perror("open input");
            exit(EXIT_FAILURE);
          }
          dup2(fd, STDIN_FILENO);
          close(fd);

          args[i] = NULL;
        } else {
          fprintf(stderr, "Syntax error: no file provided\n");
          exit(EXIT_FAILURE);
        }
      }

      else {
        i++;
      }
    }

    execvp(args[0], args);
    perror("Command Not Found");
    exit(EXIT_FAILURE);
  }

  else if (pid > 0 ){
    if(!background){
      waitpid(pid, NULL, 0);
    }
    else {
      printf("Background PID: %d\n", pid);
    }
  }

  else {
    perror("Fork Failed");
  }
}
