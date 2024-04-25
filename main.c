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
// 2. (OK) try open each redir_in, throw err msg if doesn't exist, set active one to fd_in
// 3. (OK) for heredoc type redir_in, create helper func to receive
// 4. (OK) detect builtin cmd, send to its func to exec
// 5. (OK) for external cmd, check if fullpath is executable
// 6. (OK) try open each redir_out (create if not yet), trunc silent ones, set active one to fd_out
// 7. (OK) error msg: command not found (manual), permission denied (perror), no such file or dir (perror)
// 8. unquote heredoc delimiter word, and create independent heredoc for each cmd
// 9. set exit status 2 (misused builtin), 128 (invalid argument to exit), and 128+n (fatal error signal)
// 10. handle signals
// 11. debug builtins
// 12. clean tmp heredoc files after each line executed
// 13. clean leaks
// 14. norminette