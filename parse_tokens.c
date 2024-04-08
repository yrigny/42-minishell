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

void    parse_redir_in(char *infile, t_proc *cmd, t_token **tokens)
{
    if (cmd->redir_in)
        free(cmd->redir_in);
    cmd->redir_in = ft_strdup(infile);
    *tokens = (*tokens)->next->next;
}

void    parse_redir_out(char *outfile, t_proc *cmd, t_token **tokens)
{
    char    *new_file;
    t_list  *new_node;

    new_file = ft_strdup(outfile);
    if (new_file)
        new_node = ft_lstnew(new_file);
    if (new_node)
        ft_lstadd_back(cmd->fd_out, new_node);
    *tokens = (*tokens)->next->next;
}

// to be modified
void    parse_cmd_and_arg(t_token *start, t_proc *cmd, t_token **tokens)
{
    int wordcount;
    char    **ptr;

    wordcount = 0;
    while (start && start->type == TOKEN_WORD)
    {
        wordcount++;
        start = start->next;
    }
    cmd->cmd_arr = malloc(sizeof(char *) * (wordcount + 1));
    ptr = cmd->cmd_arr;
    start = *tokens;
    while (start && start->type == TOKEN_WORD)
    {
        *ptr = ft_strdup(start->value);
        // printf("%s\n", *ptr);
        start = start->next;
        ptr++;
    }
    *ptr = NULL;
    *tokens = start;
}

void    print_cmd_arr(char **cmd_arr)
{
    printf("cmd_arr:\n");
    while (*cmd_arr)
    {
        printf("%s\n", *cmd_arr);
        cmd_arr++;
    }
}

void    print_redir_outs(t_list *outfiles)
{
    while (outfiles->next)
    {
        printf("silent outfile: %s\n", outfiles->content);
        outfiles = outfiles->next;
    }
    printf("active outfile: %s\n", outfiles->content);
}

t_list  *parse_cmd(t_token *tokens)
{
    t_proc  *new_cmd;

    new_cmd = malloc(sizeof(t_proc));
    if (!new_cmd)
        return (NULL);
    new_cmd->cmd_arr = NULL;
    new_cmd->fullpath = NULL;
    new_cmd->redir_in = NULL;
    new_cmd->redir_heredoc = NULL;
    new_cmd->redir_out = NULL;
    new_cmd->redir_append = NULL;
    while (tokens)
    {
        if (tokens->type == TOKEN_REDIR_IN)
            parse_redir_in(tokens->next->value, new_cmd, &tokens);
        // else if (tokens->type == TOKEN_REDIR_HEREDOC)
        //     parse_redir_heredoc(tokens->next, new_cmd, &tokens);
        else if (tokens->type == TOKEN_REDIR_OUT)
            parse_redir_out(tokens->next->value, new_cmd, &tokens);
        // else if (tokens->type == TOKEN_REDIR_APPEND)
        //     parse_redir_append(tokens->next, new_cmd, &tokens);
        else
            parse_cmd_and_arg(tokens, new_cmd, &tokens);
    }
    print_cmd_arr(new_cmd->cmd_arr);
    printf("infile: %s\n", new_cmd->redir_in);
    print_redir_outs(new_cmd->redir_out);
    return (ft_lstnew(new_cmd));
}

void    move_to_next_cmd(t_token **tokens)
{
    while (*tokens)
    {
        if (*tokens == NULL || (*tokens)->type == TOKEN_PIPE)
            break ;
        *tokens = (*tokens)->next;
    }
    if (*tokens && (*tokens)->type == TOKEN_PIPE)
        *tokens = (*tokens)->next;
}

t_list  *gen_exec_list(t_token *tokens)
{
    t_list  *exec_list;
    t_list  *exec_node;

    exec_list = NULL;
    if (!tokens)
        return (NULL);
    while (tokens)
    {
        exec_node = parse_cmd(tokens);
        ft_lstadd_back(&exec_list, exec_node);
        move_to_next_cmd(&tokens);
    }
    return (exec_list);
}

// void    print_exec_list(exec_list)
// {
//     t_proc  *cmd;

//     cmd = exec_list->content;
//     while ()
// }

t_list *parse_tokens(t_token **tokens)
{
    // t_shell result;
    t_list  *exec_list;
    t_token *head;

    exec_list = gen_exec_list(*tokens);
    // result.pipe_list = gen_pipe_list(*tokens);
    head = *tokens;
    while (*tokens)
    {
        free((*tokens)->value);
        (*tokens) = (*tokens)->next;
    }
    free(head);
    // print_exec_list(exec_list);
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