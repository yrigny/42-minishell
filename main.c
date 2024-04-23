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

// Todo:
// 1. (OK) pre-expand tokens before parsing tokens
// 2. try open each redir_in, throw err msg if doesn't exist, set active one to fd_in
// 3. for heredoc type redir_in, create helper func to receive
// 4. detect builtin cmd, send to its func to exec
// 5. for external cmd, check if fullpath is executable
// 6. try open each redir_out (create if not yet), empty silent ones, set active one to fd_out
