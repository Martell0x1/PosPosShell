#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#define MAX_INPUT 2048
#define MAX_ARGS 64
#define MAX_HISTORY 100
#define HISTORY_FILE ".myShell_history"
extern char history[MAX_HISTORY][MAX_INPUT];
extern int history_count;

void init_shell();
void print_prompt();
void execute_command(char* input);
int parse_args(char* input , char** args);
int handle_commands(int args_count,char** args);
void execute_a_fork_family(char** args , int background);
void add_history(char *input);
void print_history(); 
void load_history();
void save_history(char *command);
