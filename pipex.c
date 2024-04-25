/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 13:51:14 by yrigny            #+#    #+#             */
/*   Updated: 2024/04/25 13:13:25 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    pipex(t_ms *ms, t_list *cmds)
{
    int     i;
    int     nb_cmds;
	int		status;

    i = -1;
    nb_cmds = ft_lstsize(cmds);
    while (++i < nb_cmds - 1)
    {
        if (pipe(ms->pipe[i].fd) == -1)
            exit(EXIT_FAILURE);
    }
    fork_children(nb_cmds, ms->pipe, cmds);
    close(ms->pipe[i - 1].fd[0]);
    close(ms->pipe[i - 1].fd[1]);
    i = -1;
    while (++i < nb_cmds)
	{
        waitpid(ms->pipe[i].pid, &status, 0);
	}
	catch_last_status(&status);
}

void    fork_children(int nb_cmds, t_pipe pipe_arr[MAX_PIPE], t_list *cmds)
{
    int     i;

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
	handle_redir_in(child);
	handle_redir_out(child);
	dup2(child->fd_in, STDIN_FILENO);
	if (child->fd_out == STDOUT_FILENO)
		dup2(pipe[1], STDOUT_FILENO);
	else if (child->fd_out != STDOUT_FILENO)
	{
		dup2(child->fd_out, STDOUT_FILENO);
		close(pipe[1]);
	}
	close(pipe[0]);
	execute_cmd(child);
}

void	child_middle(t_cmd *child, int pipe1[2], int pipe2[2])
{
	handle_redir_in(child);
	handle_redir_out(child);
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
	execute_cmd(child);
}

void	child_last(t_cmd *child, int pipe[2])
{
	handle_redir_in(child);
	handle_redir_out(child);
	if (child->fd_in == STDIN_FILENO)
		dup2(pipe[0], STDIN_FILENO);
	else if (child->fd_in != STDIN_FILENO)
	{
		dup2(child->fd_in, STDIN_FILENO);
		close(pipe[0]);
	}
	dup2(child->fd_out, STDOUT_FILENO);
	close(pipe[1]);
	execute_cmd(child);
}
