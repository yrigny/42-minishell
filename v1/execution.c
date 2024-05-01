#include "minishell.h"

bool    cmd_exists(t_cmd *child)
{
	if (!child->fullpath)
	{
		ft_putstr_fd(child->cmd_arr[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return (0);
	}
    return (1);
}

bool	cmd_is_executable(t_cmd *child)
{
	if (access(child->fullpath, X_OK) != 0)
	{
        ft_putstr_fd("minishell: ", 2);
		perror(child->cmd_arr[0]);
		return (0);
	}
	return (1);
}

void	execute_child(t_cmd *child)
{
	int	builtin_exit_code;

	if (is_builtin(child->cmd_arr[0]))
	{
		builtin_exit_code = exec_builtin(child);
		child_free_exit(builtin_exit_code); // 0: OK, 1: general error, 2: invalid option
	}
	if (!cmd_exists(child))
		child_free_exit(127);
	if (!cmd_is_executable(child))
		child_free_exit(126);
	execve(child->fullpath, child->cmd_arr, NULL);
	perror("minishell");
	child_free_exit(1);
}

void	child_free_exit(int exit_code)
{
	free_cmd_list();
	free_env();
	rl_clear_history();
	exit(exit_code);
}

int		exec_builtin(t_cmd *child)
{
	char	*cmd_name;
	t_ms	*ms;
	int		exit_code;

	cmd_name = child->cmd_arr[0];
	ms = get_ms();
	exit_code = 0;
	if (!ft_strncmp(cmd_name, "echo", 5))
		exit_code = ft_echo(child->cmd_arr);
	else if (!ft_strncmp(cmd_name, "cd", 3))
		exit_code = ft_cd(child->cmd_arr, ms->env);
	else if (!ft_strncmp(cmd_name, "pwd", 4))
		exit_code = ft_pwd();
	else if (!ft_strncmp(cmd_name, "export", 7))
		exit_code = ft_export(child->cmd_arr, ms->env);
	else if (!ft_strncmp(cmd_name, "unset", 6))
		exit_code = ft_unset(child->cmd_arr, ms->env);
	else if (!ft_strncmp(cmd_name, "env", 4))
		exit_code = print_env(ms->env);
	else if (!ft_strncmp(cmd_name, "exit", 5))
        	ft_exit();
	return (exit_code);
}

// void	catch_last_status(int *status)
// {
// 	if (WIFEXITED(*status))
// 	{
// 		get_ms()->last_exit = WEXITSTATUS(*status);
// 		// printf("Exit status of the child was %d\n", get_ms()->last_exit);
// 	}
// 	else if (WIFSIGNALED(*status))
// 	{
// 		get_ms()->last_exit = WTERMSIG(*status);
// 		// printf("Exit status of the child was %d\n", get_ms()->last_exit);
// 	}
// }
