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

void	execute_cmd(t_cmd *child)
{
	if (!is_builtin(child->cmd_arr[0]) && !cmd_exists(child))
		exit(127);
	if (!is_builtin(child->cmd_arr[0]) && !cmd_is_executable(child))
		exit(126);
	if (is_builtin(child->cmd_arr[0]))
		exec_builtin(child);
	else
	{
		execve(child->fullpath, child->cmd_arr, NULL);
		perror("minishell");
		exit(EXIT_FAILURE);
	}
}

void	exec_builtin(t_cmd *child)
{
	char	*cmd_name;
	t_ms	*ms;

	cmd_name = child->cmd_arr[0];
	ms = get_ms();
	if (!ft_strncmp(cmd_name, "echo", 5))
		ft_echo(child->cmd_arr);
	if (!ft_strncmp(cmd_name, "cd", 3))
        ft_cd(child->cmd_arr, ms->env);
    if (!ft_strncmp(cmd_name, "pwd", 4))
        ft_pwd();
    if (!ft_strncmp(cmd_name, "export", 7))
        ft_export(child->cmd_arr, ms->env);
    if (!ft_strncmp(cmd_name, "unset", 6))
        ft_unset(child->cmd_arr, ms->env);
    if (!ft_strncmp(cmd_name, "env", 4))
        print_env(ms->env);
    if (!ft_strncmp(cmd_name, "exit", 5))
	{
        ft_exit(ms);
	}
	exit(EXIT_SUCCESS);
}

void	catch_last_status(int *status)
{
	if (WIFEXITED(*status))
	{
		get_ms()->last_exit = WEXITSTATUS(*status);
		printf("Exit status of the child was %d\n", get_ms()->last_exit);
	}
	else if (WIFSIGNALED(*status))
	{
		get_ms()->last_exit = WTERMSIG(*status);
		printf("Exit status of the child was %d\n", get_ms()->last_exit);
	}
}
