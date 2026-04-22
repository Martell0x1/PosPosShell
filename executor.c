#include "shell.h"

void execute_pipe(char *input) {

char input_cpy[MAX_INPUT];
strcpy(input_cpy, input);

char *pipe_pos = strchr(input_cpy, '|');

if (pipe_pos == NULL) return;

*pipe_pos = '\0';

char *cmd1 = input_cpy;
char *cmd2 = pipe_pos + 1;

while (*cmd2 == ' ') cmd2++;

char *args1[MAX_ARGS];
char *args2[MAX_ARGS];

parse_args(cmd1, args1);
parse_args(cmd2, args2);

int fd[2];

if(pipe(fd) == -1){
    perror("pipe failed");
    return;
}

pid_t p1 = fork();

if (p1 < 0){
    perror("fork failed");
    return;
}

if (p1 == 0) {
    if(dup2(fd[1], STDOUT_FILENO) == -1){
        perror("dup2 failed");
        exit(EXIT_FAILURE);
    }

    close(fd[0]);
    close(fd[1]);

    execvp(args1[0], args1);
    perror("command 1 failed");
    exit(EXIT_FAILURE);
}

pid_t p2 = fork();

if (p2 < 0){
    perror("fork failed");
    return;
}

if (p2 == 0) {
    if(dup2(fd[0], STDIN_FILENO) == -1){
        perror("dup2 failed");
        exit(EXIT_FAILURE);
    }

    close(fd[1]);
    close(fd[0]);

    execvp(args2[0], args2);
    perror("command 2 failed");
    exit(EXIT_FAILURE);
}

close(fd[0]);
close(fd[1]);

waitpid(p1, NULL, 0);
waitpid(p2, NULL, 0);

}

void execute_a_fork_family(char ** args , int background){

pid_t pid = fork();

if(pid < 0){
    perror("fork failed");
    return;
}

if(pid == 0){

    int i = 0;

    while (args[i] != NULL) {

        if (strcmp(args[i], ">") == 0) {

            if (args[i+1] == NULL){
                fprintf(stderr, "syntax error: no output file\n");
                exit(EXIT_FAILURE);
            }

            int fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

            if (fd < 0){
                perror("output file error");
                exit(EXIT_FAILURE);
            }

            if(dup2(fd, STDOUT_FILENO) < 0){
                perror("dup2 failed");
                exit(EXIT_FAILURE);
            }

            close(fd);
            args[i] = NULL;
        }

        else if (strcmp(args[i], "<") == 0) {

            if (args[i+1] == NULL){
                fprintf(stderr, "syntax error: no input file\n");
                exit(EXIT_FAILURE);
            }

            int fd = open(args[i+1], O_RDONLY);

            if (fd < 0){
                perror("input file error");
                exit(EXIT_FAILURE);
            }

            if(dup2(fd, STDIN_FILENO) < 0){
                perror("dup2 failed");
                exit(EXIT_FAILURE);
            }

            close(fd);
            args[i] = NULL;
        }

        i++;
    }

    execvp(args[0], args);

    fprintf(stderr, "%s: command not found\n", args[0]);
    exit(EXIT_FAILURE);
}

else {
    if(!background){
        waitpid(pid, NULL, 0);
    } else {
        printf("Background PID: %d\n", pid);
    }
}

}

void execute_command(char *input) {

if(input == NULL || strlen(input) == 0)
    return;

if (strchr(input, '|') != NULL) {
    execute_pipe(input);
    return;
}

char* args[MAX_ARGS];

int arg_count = parse_args(input, args);

if(arg_count == 0)
    return;

if(handle_commands(arg_count, args))
    return;

int background = 0;

while(arg_count > 0 && args[arg_count -1] != NULL
&& strcmp(args[arg_count -1], "&")==0){
background = 1;
args[arg_count -1] = NULL;
arg_count--;
}
execute_a_fork_family(args, background);


}
