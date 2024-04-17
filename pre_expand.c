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
    t_list  *str_node;
    int     dollar_pos;

    str_node = *cmd_arg;
    while (str_node)
    {
        dollar_pos = 0;
        while (has_expandable_dollar_str(str_node, &dollar_pos))
            expand_env_var(&str_node, dollar_pos);
        remove_quotes(&str_node, (char *)str_node->content);
        str_node = str_node->next;
    }
    expand_cmd_path(cmd, (*cmd_arg)->content);
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
void    remove_quotes(t_list **str_node, char *old_str)
{
    int     i;
    int     pair_of_quotes;
    char    quote;
    char    *new_str;

    i = -1;
    pair_of_quotes = 0;
    while (old_str[++i])
    {
        if (old_str[i] == '\'' || old_str[i] == '\"')
        {
            quote = old_str[i];
            i += 1;
            while (old_str[i] != quote)
                i++;
            pair_of_quotes += 1;
        }
    }
    new_str = assemble_new_str2(old_str, pair_of_quotes);
    free(old_str);
    (*str_node)->content = new_str;
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
