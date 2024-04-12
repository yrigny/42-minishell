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

bool    reform_as_cmd_arr(t_list *cmd_arg, t_cmd *cmd)
{
    char    **ptr;
    int     wordcount;

    wordcount = ft_lstsize(cmd_arg);
    if (!wordcount)
        return (0);
    cmd->cmd_arr = malloc(sizeof(char *) * (wordcount + 1));
    if (!(cmd->cmd_arr))
        return (0);
    ptr = cmd->cmd_arr;
    while (cmd_arg)
    {
        *ptr = ft_strdup((char *)(cmd_arg->content));
        if (!(*ptr))
            free_str_arr(&cmd->cmd_arr);
        cmd_arg = cmd_arg->next;
        ptr++;
    }
    *ptr = NULL;
    return (1);
}

void    free_str_arr(char ***p_str_arr)
{
    char    **head;

    head = *p_str_arr;
    if (!head)
        return ;
    while (*head)
    {
        free(*head);
        head++;
    }
    free(*p_str_arr);
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

void    free_cmd_list(void)
{
    t_ms    *ms;
    t_list  **cmd_list;
    t_list  *next_cmd_node;

    ms = get_ms();
    cmd_list = &ms->cmds;
    while (*cmd_list)
    {
        next_cmd_node = (*cmd_list)->next;
        free_str_arr(&((t_cmd *)(*cmd_list)->content)->cmd_arr);
        free_tokens(&((t_cmd *)(*cmd_list)->content)->redir_in);
        free_tokens(&((t_cmd *)(*cmd_list)->content)->redir_out);
        free((*cmd_list)->content);
        free(*cmd_list);
        *cmd_list = next_cmd_node;
    }
}