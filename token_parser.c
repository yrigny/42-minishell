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

// typedef struct s_pipe
// {
//     int     fd_pair[2];
//     int     pid;
//     t_proc  *process_l;
//     t_proc  *process_r;
// }   t_pipe;

// t_list  *new_pipe_node(t_token *head_of_left_cmd, t_token *head_of_right_cmd)
// {
//     t_pipe  *new;
//     t_list  node_on_list;

//     new = malloc(sizeof(t_pipe));
//     if (!new)
//         return (NULL);
//     new->process_l = parse_cmd(head_of_left_cmd);
//     new->process_r = parse_cmd(head_of_right_cmd);
//     if (!new->process_l || !new->process_r)
//         return (NULL);
//     node_on_pipe_list = ft_lstnew(new);
//     return (node_on_pipe_list);
// }

bool    parse_redir(char *file, t_token **redir_list, t_token **tokens)
{
    t_token *new_file;
    t_token *tmp;

    new_file = malloc(sizeof(t_token));
    if (!new_file)
        return (0);
    new_file->next = NULL;
    new_file->type = (*tokens)->type;
    new_file->value = ft_strdup(file);
    if (!(new_file->value))
        return (0);
    if (!(*redir_list))
        *redir_list = new_file;
    else
    {
        tmp = *redir_list;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_file;
    }
    *tokens = (*tokens)->next->next;
    return (1);
}

bool    parse_cmd_and_arg(t_list **cmd_arg, t_token **tokens)
{
    char    *new_word;
    t_list  *new_node;

    new_word = ft_strdup((*tokens)->value);
    if (!new_word)
        return (0);
    new_node = ft_lstnew(new_word);
    if (!new_node)
    {
        free(new_word);
        return (0);
    }
    ft_lstadd_back(cmd_arg, new_node);
    *tokens = (*tokens)->next;
    return (1);
}

t_list  *parse_cmd(t_token **tokens)
{
    t_proc  *new_cmd;
    t_list  *cmd_arg;

    cmd_arg = NULL;
    new_cmd = ft_calloc(1, sizeof(t_proc));
    if (!new_cmd)
        return (NULL);
    while (*tokens && (*tokens)->type != TOKEN_PIPE)
    {
        if ((*tokens)->type == TOKEN_REDIR_IN || (*tokens)->type == TOKEN_REDIR_HEREDOC)
            parse_redir((*tokens)->next->value, &new_cmd->redir_in, tokens);
        else if ((*tokens)->type == TOKEN_REDIR_OUT || (*tokens)->type == TOKEN_REDIR_APPEND)
            parse_redir((*tokens)->next->value, &new_cmd->redir_out, tokens);
        else if ((*tokens)->type == TOKEN_WORD)
            parse_cmd_and_arg(&cmd_arg, tokens);
    }
    reform_as_cmd_arr(cmd_arg, new_cmd);
    free_cmd_arg_list(&cmd_arg); // never skip this
    return (ft_lstnew(new_cmd));
}

t_list  *gen_exec_list(t_token **tokens)
{
    t_list  *exec_list;
    t_list  *exec_node;

    exec_list = NULL;
    if (!(*tokens))
        return (NULL);
    while (*tokens)
    {
        exec_node = parse_cmd(tokens); // overwrite ?
        if (!exec_node)
            return (NULL);
        ft_lstadd_back(&exec_list, exec_node);
        if (*tokens && (*tokens)->type == TOKEN_PIPE)
            *tokens = (*tokens)->next;
    }
    return (exec_list);
}

t_list *parse_tokens(t_token **tokens)
{
    // t_shell result;
    t_list  *exec_list;

    exec_list = gen_exec_list(tokens);
    if (!exec_list)
    {
        ft_putstr_fd("Token-parsing failed - malloc error.", 2);
        return (NULL);
    }
    // result.pipe_list = gen_pipe_list(*tokens);
    free_tokens(tokens); // doesn't work
    printf("%p\n", *tokens);
    print_exec_list(exec_list);
    return (exec_list);
}
