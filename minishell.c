#include "minishell.h"
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

int main(int argc, char **argv, char **envp)
{
    char *input;
    (void)argc;
    (void)argv;
    (void)envp;
    while(1)
    {
        input = readline("iogul@k2m15s01 ~/Masaüstü/minishell ");
        if(!input)
            return (0);
        if(*input)
           add_history(input);
        printf("girdi : %s\n", input);
    }

}