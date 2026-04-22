#include "shell.h"

char history[MAX_HISTORY][MAX_INPUT];
int history_count = 0;

void load_history()
{
    FILE *file = fopen(HISTORY_FILE, "r");

    if (!file)
        return;

    while (fgets(history[history_count], MAX_INPUT, file))
    {
        history_count++;

        if (history_count >= MAX_HISTORY)
            break;
    }

    fclose(file);
}

void save_history(char *command)
{
    FILE *file = fopen(HISTORY_FILE, "a");

    if (!file)
        return;

    fprintf(file, "%s", command);

    fclose(file);
}

void add_history(char *input)
{
    if (history_count < MAX_HISTORY)
    {
        strcpy(history[history_count++], input);
    }

    save_history(input);
}

void print_history()
{
    for (int i = 0; i < history_count; i++)
    {
        printf("%d %s", i + 1, history[i]);
    }
}
