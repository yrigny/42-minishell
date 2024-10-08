#include "minishell.h"

void	pipex(t_ms *ms, t_list *cmds)
{
	int	i;
	int	nb_cmds;
	int	status;

	i = -1;
	nb_cmds = ft_lstsize(cmds);
	while (++i < nb_cmds - 1)
	{
		if (pipe(ms->pipe[i].fd) == -1)
			exit(EXIT_FAILURE);
	}
	handle_redirections(cmds);
	fork_children(nb_cmds, ms->pipe, cmds);
	close(ms->pipe[i - 1].fd[0]);
	close(ms->pipe[i - 1].fd[1]);
	i = -1;
	while (++i < nb_cmds)
		waitpid(ms->pipe[i].pid, &status, 0);
	// catch_last_status(&status);
	if (WIFEXITED(status))
		get_ms()->last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		get_ms()->last_exit = WTERMSIG(status);
}

void	close_unrelated_fds(int i, int nb_pipes, t_pipe pipe_arr[MAX_PIPE])
{
	int	first_pipe_idx;
	int	last_pipe_idx;

	first_pipe_idx = 0;
	last_pipe_idx = nb_pipes - 1;
	// close all unrelated fds to the left, until the first pipe
	int x = i - 2;
	while (x >= first_pipe_idx)
	{
		printf("for cmd[%d], closing pipe[%d]\n", i, x);
		close(pipe_arr[x].fd[0]);
		close(pipe_arr[x].fd[1]);
		x--;
	}
	// close all unrelated fds to the right, until the last pipe
	int y = i + 1;
	while (y <= last_pipe_idx)
	{
		printf("for cmd[%d], closing pipe[%d]\n", i, y);
		close(pipe_arr[y].fd[0]);
		close(pipe_arr[y].fd[1]);
		y++;
	}
}

void	fork_children(int nb_cmds, t_pipe pipe_arr[MAX_PIPE], t_list *cmds)
{
	int	i;

	i = -1;
	while (++i < nb_cmds)
	{
		if (i > 1)
		{
			close(pipe_arr[i - 2].fd[0]);
			close(pipe_arr[i - 2].fd[1]);
		}
		pipe_arr[i].pid = fork();
		if (pipe_arr[i].pid == -1)
			exit(EXIT_FAILURE);
		if (pipe_arr[i].pid == 0 && i == 0)
			child_first(cmds->content, pipe_arr[0].fd);
		else if (pipe_arr[i].pid == 0 && i < nb_cmds - 1)
			child_middle(cmds->content, pipe_arr[i - 1].fd, pipe_arr[i].fd);
		else if (pipe_arr[i].pid == 0 && i == nb_cmds - 1)
			child_last(cmds->content, pipe_arr[i - 1].fd);
		cmds = cmds->next;
	}
}

void	child_first(t_cmd *child, int pipe[2])
{
	dup2(child->fd_in, STDIN_FILENO);
	if (child->fd_out == STDOUT_FILENO)
		dup2(pipe[1], STDOUT_FILENO);
	else if (child->fd_out != STDOUT_FILENO)
	{
		dup2(child->fd_out, STDOUT_FILENO);
		close(pipe[1]);
	}
	close(pipe[0]);
	execute_child(child);
}

void	child_middle(t_cmd *child, int pipe1[2], int pipe2[2])
{
	if (child->fd_in == STDIN_FILENO)
		dup2(pipe1[0], STDIN_FILENO);
	else if (child->fd_in != STDIN_FILENO)
	{
		dup2(child->fd_in, STDIN_FILENO);
		close(pipe1[0]);
	}
	if (child->fd_out == STDOUT_FILENO)
		dup2(pipe2[1], STDOUT_FILENO);
	else if (child->fd_out != STDOUT_FILENO)
	{
		dup2(child->fd_out, STDOUT_FILENO);
		close(pipe2[1]);
	}
	close(pipe1[1]);
	close(pipe2[0]);
	execute_child(child);
}

void	child_last(t_cmd *child, int pipe[2])
{
	if (child->fd_in == STDIN_FILENO)
		dup2(pipe[0], STDIN_FILENO);
	else if (child->fd_in != STDIN_FILENO)
	{
		dup2(child->fd_in, STDIN_FILENO);
		close(pipe[0]);
	}
	dup2(child->fd_out, STDOUT_FILENO);
	close(pipe[1]);
	execute_child(child);
}
