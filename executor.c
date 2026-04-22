#include "shell.h";

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
