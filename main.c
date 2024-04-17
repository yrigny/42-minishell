/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:16:09 by ael-mank          #+#    #+#             */
/*   Updated: 2024/03/13 09:49:00 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int		g_signal;

int	main(int argc, char **argv, char **envp)
{
	t_ms *ms;

	(void)argc;
	(void)argv;
	ms = get_ms();
	// g_signal = 0;
	if (init_env(envp, ms) == FAILURE)
		return (FAILURE);
	// ms->curr_dir = get_curr_dir();
	shell_routine();
	free_env(ms->env);
	return (SUCCESS);
}