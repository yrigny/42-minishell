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
    int     expand_pos;

    expand_cmd_path(cmd, (*cmd_arg)->content);
    // expand env_var below
    if (ft_lstsize(*cmd_arg) <= 1)
        return ;
    args = (*cmd_arg)->next;
    while (args)
    {
        while ((expand_pos = has_expandable_dollar_str(args)) > 0)
            expand_env_var(&args, expand_pos);
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
    paths = get_paths_list();
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

char    **get_paths_list(void)
{
    t_list  *env;
    char    **paths;

    env = get_ms()->env;
    while (env && !ft_strncmp((t_env *)(env->content)->name, "PATH", 5))
        env = env->next;
    if (env && ft_strncmp((t_env *)(env->content)->name, "PATH", 5))
        paths = ft_split((t_env *)(env->content)->value, ':');
    else
        paths = NULL;
    return (paths);
}

int     has_expandable_dollar_str(t_list *arg)
{
    char    *str;
    int     single_quote_open;

    str = (char *)(arg->content);
    single_quote_open = 0;
    while (*str)
    {
        if (*str == '\'')
            single_quote_open = (single_quote_open + 1) % 2;
        else if (*str == '$' && !single_quote_open && *(str + 1) && *(str + 1) != ' ')
            return (str - arg->content + 1);
        str++;
    }
    return (0);
}

void    expand_env_var(t_list **cmd_arg, int start)
{
    int     end;
    
}