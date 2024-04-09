/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parser_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 20:23:28 by yrigny            #+#    #+#             */
/*   Updated: 2024/04/09 20:23:30 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int new_proc_node(t_proc **ptr)
{ 
    *ptr = malloc(sizeof(t_proc));
    if (!(*ptr))
        return (0);
    (*ptr)->cmd_arr = NULL;
    (*ptr)->fullpath = NULL;
    (*ptr)->redir_in = NULL;
    (*ptr)->redir_heredoc = NULL;
    (*ptr)->redir_out = NULL;
    (*ptr)->redir_append = NULL;
    (*ptr)->fd_in = 0;
    (*ptr)->fd_out = 0;
    return (1);
}

void    free_cmd_arg_list(t_list **cmd_arg)
{
    t_list  *next;

    while (*cmd_arg)
    {
        next = (*cmd_arg)->next;
        free((*cmd_arg)->content);
        free(*cmd_arg);
        *cmd_arg = next;
    }
}

void    print_exec_list(t_list *exec_list)
{
    t_proc  *exec_node;
    t_list  *outfiles;
    char    **str_arr;

    while (exec_list)
    {
        exec_node = (t_proc *)(exec_list->content);
        // print infile
        printf("infile: %s\n", exec_node->redir_in);
        // print outfile(s)
        outfiles = exec_node->redir_out;
        while (outfiles && outfiles->next)
        {
            printf("silent outfile: %s\n", (char *)(outfiles->content));
            outfiles = outfiles->next;
        }
        if (outfiles)
            printf("active outfile: %s\n", (char *)(outfiles->content));
        // print cmd with args
        str_arr = exec_node->cmd_arr;
        while (*str_arr)
        {
            printf("%s\n", *str_arr);
            str_arr++;
        }
        exec_list = exec_list->next;
    }
}

void    free_cmd_arr(char ***p_cmd_arr)
{
    char    **head;

    head = *p_cmd_arr;
    if (!head)
        return ;
    while (*head)
    {
        free(*head);
        head++;
    }
    free(*p_cmd_arr);
}
