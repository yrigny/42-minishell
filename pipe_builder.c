/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 13:51:14 by yrigny            #+#    #+#             */
/*   Updated: 2024/04/15 13:51:15 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_first(t_cmd *child, int pipe[2])
{
	child->fd_in = open_redir_in();
	if (child->fd_in == -1)
		return ;
	// if (!child->fullpath)
	// {
	// 	ft_putstr_fd(child->cmd_arr[0], 2);
	// 	ft_putstr_fd(": command not found\n", 2);
	// 	return ;
	// }
	// if (access(child->path, X_OK) != 0)
	// {
	// 	perror(child->cmd_arr[0]);
	// 	return ;
	// }
	dup2(child->fd_in, STDIN_FILENO);
	dup2(pipe[1], STDOUT_FILENO);
	close(pipe[0]);
	execve(child->path, child->cmd_arr, NULL);
	perror("minishell");
}

void	child_middle(t_cmd *child, int pipe1[2], int pipe2[2])
{
	// if (!child->path)
	// {
	// 	ft_putstr_fd(child->cmd_arr[0], 2);
	// 	ft_putstr_fd(": command not found\n", 2);
	// 	return ;
	// }
	// if (access(child->path, X_OK) != 0)
	// {
	// 	perror(child->cmd_arr[0]);
	// 	return ;
	// }
	dup2(pipe1[0], STDIN_FILENO);
	dup2(pipe2[1], STDOUT_FILENO);
	close(pipe1[1]);
	close(pipe2[0]);
	// execve(child->path, child->cmd_arr, NULL);
	perror("minishell");
}

void	child_last(t_cmd *child, int pipe[2])
{
	// fd_dst = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	// if (!child->path)
	// {
	// 	ft_putstr_fd(child->cmd_arr[0], 2);
	// 	ft_putstr_fd(": command not found\n", 2);
	// 	return ;
	// }
	// if (access(child->path, X_OK) != 0)
	// {
	// 	perror(child->cmd_arr[0]);
	// 	return ;
	// }
	dup2(pipe[0], STDIN_FILENO);
	dup2(fd_dst, STDOUT_FILENO);
	close(pipe[1]);
	// execve(child->path, child->cmd_arr, NULL);
	perror("minishell");
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
            free_exit(FAILURE);
        if (pipe_arr[i].pid == 0 && i == 0)
            child_first(cmds->content, pipe_arr[0].fd);
        else if (pipe_arr[i].pid == 0 && i < nb_cmds - 1)
            child_middle(cmds->content, pipe_arr[i - 1].fd, pipe_arr[i].fd);
        else if (pipe_arr[i].pid == 0 && i == nb_cmds - 1)
            child_last(cmds->content, pipe_arr[i - 1].fd);
        if (pipe_arr[i].pid == 0)
            free_exit(FAILURE);
        cmds = cmds->next;
    }
}

void    pipex(t_ms *ms, t_list *cmds)
{
    int     i;
    int     nb_cmds;
    // t_pipe  pipebook[MAX_PIPE];

    i = -1;
    nb_cmds = ft_lstsize(cmds);
    while (++i < nb_cmds - 1)
    {
        if (pipe(ms->pipe[i].fd) == -1)
            free_exit(FAILURE);
    }
    fork_children(nb_cmds, ms->pipe, cmds);
    close(ms->pipe[i - 1].fd[0]);
    close(ms->pipe[i - 1].fd[1]);
    i = -1;
    while (++i < nb_cmds)
        waidpid(ms->pipe[i].pid, NULL, 0);
}