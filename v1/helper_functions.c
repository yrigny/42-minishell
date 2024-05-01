#include "minishell.h"

void    print_tokens(t_token *tokens)
{
    while (tokens)
    {
        printf("type: %d\tvalue: [%s]\n", tokens->type, tokens->value);
        tokens = tokens->next;
    }
}

void    print_cmd_list(t_list *cmd_list)
{
    t_cmd   *cmd_node;
    t_token *infiles;
    t_token *outfiles;
    char    **str_arr;

    while (cmd_list)
    {
        printf("----------------------\n");
        cmd_node = (t_cmd *)(cmd_list->content);
        // print infile(s)
        infiles = cmd_node->redir_in;
        while (infiles && infiles->next)
        {
            printf("silent infile: %s\n", infiles->value);
            infiles = infiles->next;
        }
        if (infiles)
            printf("active infile: %s\n", infiles->value);
        // print outfile(s)
        outfiles = cmd_node->redir_out;
        while (outfiles && outfiles->next)
        {
            printf("silent outfile: %s\n", outfiles->value);
            outfiles = outfiles->next;
        }
        if (outfiles)
            printf("active outfile: %s\n", outfiles->value);
        // print cmd fullpath
        printf("fullpath: %s\n", cmd_node->fullpath);
        // print cmd with args
        str_arr = cmd_node->cmd_arr;
        if (*str_arr)
        {
            printf("cmd: %s\n", *str_arr);
            str_arr++;
        }
        while (*str_arr)
        {
            printf("arg: %s\n", *str_arr);
            str_arr++;
        }
        cmd_list = cmd_list->next;
        printf("----------------------\n");
    }
}
