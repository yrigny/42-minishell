/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 15:47:04 by yrigny            #+#    #+#             */
/*   Updated: 2024/04/15 15:47:06 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    pre_expand(t_list **cmd_arg, t_cmd *cmd)
{
    t_list  *args;
    int     dollar_pos;

    expand_cmd_path(cmd, (*cmd_arg)->content);
    // expand env_var below
    if (ft_lstsize(*cmd_arg) <= 1)
        return ;
    args = (*cmd_arg)->next;
    while (args)
    {
        dollar_pos = 0;
        while (has_expandable_dollar_str(args, &dollar_pos))
            expand_env_var(&args, dollar_pos);
        // remove_quotes(&args);
        args = args->next;
    }
}

void    expand_cmd_path(t_cmd *cmd, char *executable)
{
    char    **paths;
    char    *tmp_cmd_path;
    int     i;

    if (access(executable, F_OK) == 0)
    {
        cmd->fullpath = ft_strdup(executable);
        return ;
    }
    paths = get_paths_array();
    if (!paths)
        return ;
    i = -1;
    while (!cmd->fullpath && executable && paths[++i])
    {
        tmp_cmd_path = ft_strjoin(paths[i], "/");
        cmd->fullpath = ft_strjoin(tmp_cmd_path, executable);
        if (access(cmd->fullpath, F_OK) != 0)
        {
            free(cmd->fullpath);
            cmd->fullpath = NULL;
        }
        free(tmp_cmd_path);
    }
    free_str_arr(&paths);
}

char    **get_paths_array(void)
{
    t_list  *env;
    char    **paths;

    env = get_ms()->env;
    while (env && ft_strncmp(((t_env *)env->content)->name, "PATH", 5))
        env = env->next;
    if (env && !ft_strncmp(((t_env *)env->content)->name, "PATH", 5))
        paths = ft_split(((t_env *)env->content)->value, ':');
    else
        paths = NULL;
    return (paths);
}

bool    has_expandable_dollar_str(t_list *arg, int *dollar_pos)
{
    char    *str;
    int     single_quote_open;

    str = (char *)(arg->content);
    single_quote_open = 0;
    while (*str)
    {
        if (*str == '\'')
            single_quote_open = (single_quote_open + 1) % 2;
        else if (!single_quote_open && *str == '$'
                && (ft_isalnum(*(str + 1)) || *(str + 1) == '_'))
        {
            *dollar_pos = str - (char *)arg->content;
            return (1);
        }
        str++;
    }
    return (0);
}

void    expand_env_var(t_list **cmd_arg, int head)
{
    int     end;
    char    *old_str;
    char    *env_var_value;
    char    *new_str;

    old_str = (char *)((*cmd_arg)->content);
    end = head + 1;
    while (ft_isalnum(old_str[end + 1]) || old_str[end + 1] == '_')
        end++;
    env_var_value = match_env_var(&old_str[head + 1], end - head);
    // printf("env_var_value: [%s]\n", env_var_value);
    new_str = assemble_new_str(old_str, env_var_value, head, end);
    free(old_str);
    // printf("new str: %s\n", new_str);
    (*cmd_arg)->content = new_str;
}

char    *match_env_var(char *name, int len)
{
    t_list  *env;

    env = get_ms()->env;
    while (env && ft_strncmp(((t_env *)env->content)->name, name, len))
        env = env->next;
    if (!env || ft_strlen(((t_env *)env->content)->name) != (size_t)len)
        return ("");
    else
        return (((t_env *)env->content)->value);
}

char    *assemble_new_str(char *old_str, char *value, int head, int end)
{
    int     value_len;
    int     tail_len;
    int     total_len;
    char    *new_str;

    value_len = ft_strlen(value);
    tail_len = ft_strlen(old_str) - (end + 1);
    total_len = ft_strlen(old_str) - (end - head + 1) + value_len;
    new_str = malloc(total_len + 1);
    if (!new_str)
        return (NULL);
    // copy head bytes before dollar str
    ft_strlcpy(new_str, old_str, head + 1);
    // copy env_var value string
    ft_strlcpy(new_str + head, value, value_len + 1);
    // copy tail bytes after dollar str
    ft_strlcpy(new_str + head + value_len, old_str + end + 1, tail_len + 1);
    return (new_str);
}