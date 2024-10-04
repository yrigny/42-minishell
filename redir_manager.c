#include "minishell.h"

void    handle_redirections(t_list *cmds)
{
    t_cmd   *curr_cmd;

    while (cmds)
    {
        curr_cmd = (t_cmd *)cmds->content;
        handle_redir_in(curr_cmd);
        handle_redir_out(curr_cmd);
        cmds = cmds->next;
    }
}

void    handle_redir_in(t_cmd *cmd)
{
    t_token *src;
    char    *filename;

    src = cmd->redir_in;
    if (!src)
        return ;
    while (src)
    {
        if (src->type == TOKEN_REDIR_HEREDOC)
        {
            filename = gen_unique_filename((unsigned long)cmd);
            cmd->fd_in = receive_heredoc(src->value, filename);
            unlink(filename);
            free(filename);
        }
        else if (src->type == TOKEN_REDIR_IN)
            cmd->fd_in = open(src->value, O_RDONLY, 0644);
        if (cmd->fd_in == -1)
        {
            ft_putstr_fd("minishell: ", 2);
            perror(src->value);
        }
        src = src->next;
    }
}

int receive_heredoc(char *delimiter, char *filename)
{
    int     fd_tmp;
    char    *line;

    fd_tmp = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
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
    return (open(filename, O_RDONLY, 0644));
}

char    *gen_unique_filename(unsigned long p)
{
    char    address[17];
    char    *hex_char;
    char    *filename;
    int     i;

    hex_char = "0123456789abcdef";
    i = -1;
    while(++i < 16)
        address[i] = hex_char[(p >> (4 * (15 - i))) & 0xf];
    address[i] = '\0';
    filename = ft_strjoin(address, ".heredoc.tmp");
    return (filename);
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
        }
        dst = dst->next;
    }
}