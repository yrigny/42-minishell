#include "minishell.h"

// $ cat < in < Makefile
// bash: in: No such file or directory

void    handle_redir_in(t_cmd *cmd)
{
    t_token *src;

    src = cmd->redir_in;
    if (!src)
        return ;
    while (src)
    {
        if (src->type == TOKEN_REDIR_HEREDOC)
            cmd->fd_in = receive_heredoc(src->value);
        else if (src->type == TOKEN_REDIR_IN)
            cmd->fd_in = open(src->value, O_RDONLY, 0644);
        if (cmd->fd_in == -1)
        {
            ft_putstr_fd("minishell: ", 2);
            perror(src->value);
            exit(1);
        }
        src = src->next;
    }
}

int receive_heredoc(char *delimiter) // add prefix to let each cmd have its own heredoc
{
    int     fd_tmp;
    char    *line;

    fd_tmp = open(".heredoc.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    while (1)
    {
        ft_putstr_fd("> ", 1);
        line = get_next_line(0);
        if (!line)
            break ;
        if (!ft_strncmp(line, delimiter, ft_strlen(delimiter))
                && ft_strlen(line) == ft_strlen(delimiter) + 1)
        {
            free(line);
            break ;
        }
        ft_putstr_fd(line, fd_tmp);
        free(line);
    }
    close(fd_tmp);
    return (open(".heredoc.tmp", O_RDONLY, 0644));
}

void    handle_redir_out(t_cmd *cmd)
{
    t_token *dst;

    dst = cmd->redir_out;
    if (!dst)
    {
        cmd->fd_out = STDOUT_FILENO;
        return ;
    }
    while (dst)
    {
        if (dst->type == TOKEN_REDIR_APPEND)
            cmd->fd_out = open(dst->value, O_CREAT | O_WRONLY | O_APPEND, 0644);
        else if (dst->type == TOKEN_REDIR_OUT)
            cmd->fd_out = open(dst->value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (cmd->fd_out == -1)
        {
            ft_putstr_fd("minishell: ", 2);
            perror(dst->value);
            exit(1);
        }
        dst = dst->next;
    }
}