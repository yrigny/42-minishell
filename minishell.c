/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:55:53 by yrigny            #+#    #+#             */
/*   Updated: 2024/04/12 16:55:54 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	init_env(char **envp, t_ms *ms)
{
	t_env	*env_var;
	t_list	*env_var_node;
    int     i;
	int		j;

    i = -1;
	while (envp[++i])
	{
		j = 0;
		while (envp[i][j] != '=')
			j++;
		env_var = malloc(sizeof(t_env));
		if (!env_var)
			return (FAILURE);
		env_var->name = ft_substr(envp[i], 0, j);
		if (!env_var->name)
			return (FAILURE);
		env_var->value = ft_strdup(&envp[i][j + 1]);
		if (!env_var->value)
			return (FAILURE);
		env_var_node = ft_lstnew(env_var);
		if (!env_var_node)
			return (FAILURE);
		ft_lstadd_back(&ms->env, env_var_node);
	}
	return (SUCCESS);
}

void	free_env(t_list *env)
{
	t_list	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(((t_env *)tmp->content)->name);
		free(((t_env *)tmp->content)->value);
		free(tmp->content);
		free(tmp);
	}
}

void	shell_routine(void)
{
	char	*line;
	t_token	*tokens;

	while (1)
	{
		if (GNL == 0)
			line = readline("minishell> ");
		else
			line = get_next_line(0);
		if (!line)
			break ;
		if (empty_line(line))
			continue ;
		add_history(line);
		tokens = tokenize_and_check_syntax(line);
		// if (!tokens)
		// 	g_signal = 258;
		if (!tokens)
			continue ;
		parse_token_into_cmds(tokens);
		exec_manager();
		free_cmd_list();
		// update_env_status(env, status, "=?");
	}
	rl_clear_history();
}
