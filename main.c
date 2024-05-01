#include "minishell.h"

// int		g_signal;

int	main(int argc, char **argv, char **envp)
{
	t_ms	*ms;

	(void)argc;
	(void)argv;
	ms = get_ms();
	// g_signal = 0;
	if (init_env(envp, ms) == 0)
		return (EXIT_FAILURE);
	shell_routine();
	free_env();
	return (EXIT_SUCCESS);
}

// Todo:
// 1. (OK) pre-expand tokens before parsing tokens
// 2. (OK) try open each redir_in, throw err msg if doesn't exist, set active one to fd_in
// 3. (OK) for heredoc type redir_in, create helper func to receive
// 4. (OK) detect builtin cmd, send to its func to exec
// 5. (OK) for external cmd, check if fullpath is executable
// 6. (OK) try open each redir_out (create if not yet), trunc silent ones, set active one to fd_out
// 7. (OK) error msg: command not found (manual), permission denied (perror), no such file or dir (perror)
// 8. (OK) create independent heredoc for each cmd
// 9. set exit status 2 (misused builtin), and 128+n (fatal error signal)
// 10. add signals handler
// 11. (OK) export segfult when no argument
// 12. (OK) clean tmp heredoc files after each line executed
// 13. (OK) replace current prompt with customized prompt ("user:cwd$ ")
// 14. (OK) fix bug with str replacement of "$?"
// 15. (OK) free malloc'd thing in each child process
// 16. limit the use of '||' '&&' '(' ')' in syntax checker
// 17. test and debug other builtins
// 18. norminette