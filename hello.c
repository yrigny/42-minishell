
#include "minishell.h"

// int lexer(char *line)
// {

// }

int main(int argc, char **argv, char **envp)
{
    char    *line;
    // ignore Ctrl-\ Ctrl-C Ctrl-D signals
    // signal_handler();
    int     i = -1;
    if (argc && argv)
        printf("argc = %d\n", argc);
    while (envp[++i])
        printf("%s\n", envp[i]);
    while (1)
    {
        line = readline("minishell> ");
        if (!line || !strcmp(line, "exit"))
            exit(1);
        printf("line of len %ld: %s\n", strlen(line), line);
        // if (lexer(line))
        // {
        //     t_cmd   *parsed_cmd_arr = parser(line, envp);
        //     execution(parsed_cmd_arr, envp);
        // }
        free(line);
    }
    return (0);
}