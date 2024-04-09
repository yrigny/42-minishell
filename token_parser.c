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

// Ex: < in1 in2 > out1 > out2 cmd1 | cmd2

int parse_redir_in(char *infile, t_proc *cmd, t_token **tokens)
{
    if (cmd->redir_in)
        free(cmd->redir_in);
    cmd->redir_in = ft_strdup(infile);
    if (!cmd->redir_in)
        return (0);
    *tokens = (*tokens)->next->next;
    return (1);
}

int parse_redir_out(char *outfile, t_proc *cmd, t_token **tokens)
{
    char    *new_file;
    t_list  *new_node;

    new_file = ft_strdup(outfile);
    if (!new_file)
    {
        // free cmd->redir_out
        return (0);
    }
    new_node = ft_lstnew(new_file);
    if (!new_node)
    {
        // free cmd->redir_out
        return (0);
    }
    ft_lstadd_back(&cmd->redir_out, new_node);
    *tokens = (*tokens)->next->next;
    return (1);
}

int parse_cmd_and_arg(t_list **cmd_arg, t_token **tokens)
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

int reform_as_cmd_arr(t_list *cmd_arg, t_proc *cmd)
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
            free_cmd_arr(&cmd->cmd_arr);
        cmd_arg = cmd_arg->next;
        ptr++;
    }
    *ptr = NULL;
    return (1);
}

t_list  *parse_cmd(t_token **tokens)
{
    t_proc  *new_cmd;
    t_list  *cmd_arg;
    int     malloc_ok;

    cmd_arg = NULL;
    malloc_ok = new_proc_node(&new_cmd);
    while (malloc_ok && *tokens && (*tokens)->type != TOKEN_PIPE)
    {
        if (malloc_ok && (*tokens)->type == TOKEN_REDIR_IN)
            malloc_ok = parse_redir_in((*tokens)->next->value, new_cmd, tokens);
        // if (malloc_ok && tokens->type == TOKEN_REDIR_HEREDOC)
        //     parse_redir_heredoc(tokens->next, new_cmd, &tokens);
        else if (malloc_ok && (*tokens)->type == TOKEN_REDIR_OUT)
            malloc_ok = parse_redir_out((*tokens)->next->value, new_cmd, tokens);
        // if (malloc_ok && tokens->type == TOKEN_REDIR_APPEND)
        //     parse_redir_append(tokens->next, new_cmd, &tokens);
        else if (malloc_ok && (*tokens)->type == TOKEN_WORD)
            malloc_ok = parse_cmd_and_arg(&cmd_arg, tokens);
    }
    if (malloc_ok)
        malloc_ok = reform_as_cmd_arr(cmd_arg, new_cmd);
    free_cmd_arg_list(&cmd_arg); // never skip this
    if (!malloc_ok)
        return (NULL);
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
    t_token *head;

    exec_list = gen_exec_list(tokens);
    if (!exec_list)
    {
        ft_putstr_fd("Token-parsing failed - malloc error.", 2);
        return (NULL);
    }
    // result.pipe_list = gen_pipe_list(*tokens);
    head = *tokens;
    while (*tokens)
    {
        free((*tokens)->value);
        (*tokens) = (*tokens)->next;
    }
    free(head);
    print_exec_list(exec_list);
    return (exec_list);
}

// t_list  *parse_tokens(t_token **tokens)
// {
//     t_list  *exec_list;
//     t_pipe  *pipe_node;
//     t_token *token_mark;

//     exec_list = NULL;
//     pipe_node = NULL;
//     token_mark = *tokens; // head of some cmd
//     while (*tokens && (*tokens)->next)
//     {
//         if ((*tokens)->next->type == TOKEN_PIPE)
//         // create pipe node
//         {
//             *tokens = (*tokens)->next; // step on the pipe
//             // send the head of left cmd and the head of right cmd to form pipe
//             pipe_node = new_pipe_node(token_mark, (*tokens)->next);
//             ft_lstadd_back(&pipe_list, pipe_node);
//             token_mark = (*tokens)->next;
//         }
//         *tokens = (*tokens)->next;
//     }
//     if (!*tokens && !pipe_node) // pipe not found
//         exec_list = parse_cmd(token_mark);
//     return (exec_list);
// }