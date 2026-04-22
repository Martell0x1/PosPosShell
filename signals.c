#include "shell.h"

void sigint_handler(int sig)
{
    printf("\nmyShell> ");
    fflush(stdout);
}

void init_shell()
{
    signal(SIGINT, sigint_handler);
}
void print_prompt(){
  printf("myShell> ");
}
