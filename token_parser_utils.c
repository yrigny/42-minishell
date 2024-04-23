#include "minishell.h"

void    reform_as_cmd_arr(t_list *cmd_arg, t_cmd *cmd)
{
    char    **ptr;
    int     wordcount;

    wordcount = ft_lstsize(cmd_arg);
    if (!wordcount)
        return ;
    cmd->cmd_arr = malloc(sizeof(char *) * (wordcount + 1));
    if (!(cmd->cmd_arr))
        return ;
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
        free(((t_cmd *)(*cmd_list)->content)->fullpath);
        free_tokens(&((t_cmd *)(*cmd_list)->content)->redir_in);
        free_tokens(&((t_cmd *)(*cmd_list)->content)->redir_out);
        free((*cmd_list)->content);
        free(*cmd_list);
        *cmd_list = next_cmd_node;
    }
}

void	free_tokens(t_token **tokens)
{
	t_token	*temp;

    temp = NULL;
	while (*tokens)
	{
		temp = (*tokens)->next;
		free((*tokens)->value);
		free(*tokens);
		*tokens = temp;
	}
}
