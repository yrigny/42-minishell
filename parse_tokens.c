/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 16:19:48 by yrigny            #+#    #+#             */
/*   Updated: 2024/04/03 16:20:02 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_proc
{
    char    **cmd_arr;
    char    *fullpath;
    int     fd_in;
    int     fd_out;
    int     pipe[2];
}   t_proc;

typedef struct s_pipe
{
    int     fd_pair[2];
    int     pid;
    t_proc  *process_l;
    t_proc  *process_r;
}

t_proc  *parse_cmd(t_token *tokens)
{
    
}

t_list  *new_pipe_node(t_token *head_of_left_cmd, t_token *head_of_right_cmd)
{
    t_pipe  *new;
    t_list  node_on_list;

    new = malloc(sizeof(t_pipe));
    if (!new)
        return (NULL);
    new->process_l = parse_cmd(head_of_left_cmd);
    new->process_r = parse_cmd(head_of_right_cmd);
    if (!new->process_l || !new->process_r)
        return (NULL);
    node_on_pipe_list = ft_lstnew(new);
    return (node_on_pipe_list);
}

t_list  *parse_tokens(t_token **tokens)
{
    t_pipe  *pipe_node;
    t_list  *pipe_list;
    t_token *start_token;

    pipe_list = NULL;
    start_token = *tokens; // head of some cmd
    while (*tokens && (*tokens)->next)
    {
        if ((*tokens)->next->type == TOKEN_PIPE)
        // create pipe node
        {
            *tokens = (*tokens)->next; // step on the pipe
            // send the head of left cmd and the head of right cmd to form pipe
            pipe_node = new_pipe_node(start_token, (*tokens)->next);
            ft_lstadd_back(&pipe_list, pipe_node);
            start_token = (*tokens)->next;
        }
        *tokens = (*tokens)->next;
    }
    return (pipe_list);
}