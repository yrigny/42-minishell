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
        printf("%s\n", envp[i]);
		j = 0;
		while (envp[i][j] != '=')
			j++;
		env_var = malloc(sizeof(t_env));
		if (!env_var)
			return (FAILURE);
		env_var->name = ft_substr(envp[i], 0, j);
		if (!env_var->name)
			return (FAILURE);
        printf("%s\t", env_var->name);
		env_var->value = ft_strdup(&envp[i][j + 1]);
		if (!env_var->value)
			return (FAILURE);
        printf("%s\n", env_var->value);
		env_var_node = ft_lstnew(env_var);
		if (!env_var_node)
			return (FAILURE);
		ft_lstadd_back(&ms->env, env_var_node);
	}
	return (SUCCESS);
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
		tokens = check_syntax_and_tokenize(line);
		// if (!tokens)
		// 	g_signal = 258;
		parse_tokens(tokens);
		free_cmd_list();
		// if (!g_signal)
		// {
		// 	exec_list = parse_tokens(&tokens);
		// 	execution_manager(exec_list, env, &status);
		// 	free_exec_list(exec_list);
		// }
		// update_env_status(env, status, "=?");
	}
	rl_clear_history();
}
