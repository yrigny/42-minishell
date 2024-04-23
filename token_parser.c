#include "minishell.h"

void    parse_token_into_cmds(t_token *tokens)
{    
    t_ms    *ms;

    if (!tokens)
        return ;
    ms = get_ms();
    pre_expand(tokens);
    // print_tokens(tokens);
    ms->cmds = gen_cmd_list(tokens);
    if (!ms->cmds)
    {
        ft_putstr_fd("Token-parsing failed - malloc error.\n", 2);
        return ;
    }
    free_tokens(&tokens);
    expand_fullpath(ms->cmds);
    print_cmd_list(ms->cmds);
}

t_list  *gen_cmd_list(t_token *tokens)
{
    t_list  *cmd_list;
    t_list  *cmd_node;

    cmd_list = NULL;
    if (!tokens)
        return (NULL);
    while (tokens)
    {
        cmd_node = parse_cmd(&tokens);
        if (!cmd_node)
            return (NULL);
        ft_lstadd_back(&cmd_list, cmd_node);
        if (tokens && tokens->type == TOKEN_PIPE)
            tokens = tokens->next;
    }
    return (cmd_list);
}

t_list  *parse_cmd(t_token **tokens)
{
    t_cmd   *new_cmd;
    t_list  *cmd_arg;

    cmd_arg = NULL;
    new_cmd = ft_calloc(1, sizeof(t_cmd));
    if (!new_cmd)
        return (NULL);
    while (*tokens && (*tokens)->type != TOKEN_PIPE)
    {
        if ((*tokens)->type == TOKEN_REDIR_IN
                || (*tokens)->type == TOKEN_REDIR_HEREDOC)
            parse_redir((*tokens)->next->value, &new_cmd->redir_in, tokens);
        else if ((*tokens)->type == TOKEN_REDIR_OUT
                || (*tokens)->type == TOKEN_REDIR_APPEND)
            parse_redir((*tokens)->next->value, &new_cmd->redir_out, tokens);
        else if ((*tokens)->type == TOKEN_WORD)
            parse_cmd_and_arg(&cmd_arg, tokens);
    }
    reform_as_cmd_arr(cmd_arg, new_cmd);
    free_cmd_arg_list(&cmd_arg); // never skip this
    return (ft_lstnew(new_cmd));
}

void    parse_redir(char *file, t_token **redir_list, t_token **tokens)
{
    t_token *new_file;
    t_token *tmp;

    new_file = malloc(sizeof(t_token));
    if (!new_file)
        return ;
    new_file->next = NULL;
    new_file->type = (*tokens)->type;
    new_file->value = ft_strdup(file);
    if (!(new_file->value))
        return ;
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
}

void    parse_cmd_and_arg(t_list **cmd_arg, t_token **tokens)
{
    char    *new_word;
    t_list  *new_node;

    new_word = ft_strdup((*tokens)->value);
    if (!new_word)
        return ;
    new_node = ft_lstnew(new_word);
    if (!new_node)
    {
        free(new_word);
        return ;
    }
    ft_lstadd_back(cmd_arg, new_node);
    *tokens = (*tokens)->next;
}
