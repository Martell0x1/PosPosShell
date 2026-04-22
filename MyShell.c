#include "shell.h"

int main(){

init_shell();

char input[MAX_INPUT];

load_history();

while(1){

    print_prompt();

    if(!fgets(input , MAX_INPUT , stdin)){
        clearerr(stdin);
        continue;
    }

    if(strlen(input) == 1)
        continue;

    add_history(input);

    execute_command(input);
}

}
