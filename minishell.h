/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:36:17 by ael-mank          #+#    #+#             */
/*   Updated: 2024/03/13 09:48:07 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>	
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef enum e_token_type
{
    TOKEN_WORD,         // cmd and args
    TOKEN_PIPE,         // '|'
    TOKEN_REDIR_IN,     // '<'
    TOKEN_REDIR_HEREDOC,// '<<'
    TOKEN_REDIR_OUT,    // '>'
    TOKEN_REDIR_APPEND, // '>>'
    TOKEN_ENV_VAR,      // '$'
}   t_token_type;

typedef struct s_token
{
    t_token_type    type;
    char            *value;
    struct s_token  *next;
}   t_token;

typedef struct s_proc
{
    char    **cmd_arr; // filled by token parser
    char    *fullpath; // to do later in pre-execution step
    t_token *redir_in; // filled by token parser
    t_token *redir_out; // filled by token parser
    int     fd_in; // to do in pipe parser
    int     fd_out; // to do in pipe parser
    // int  pipe[2];
}   t_proc;

typedef struct s_shell
{
    t_list  *exec_list;
    t_list  *pipe_list;
}   t_shell;

/* syntax check */
int		ft_isspace(char c);
char	*trim_line(char *str);
int		syntax_error(char *line);
int		has_unclosed_quote(char *line);
int		has_invalid_redir(char *line);
int		has_misplaced_oparator(char *line);
int		has_logical_oparator(char *line);
void	update_open_quote(char **line, int *single_quote, int *double_quote);
void	skip_space(char **line);
void	skip_quoted(char **line);

/* tokenization */
t_token	*tokenize(char *line);
t_token	*new_word(char *start, char **end);
t_token	*new_metacharacter(char *start, char **end);
t_token	*new_token(t_token_type type, char *value);
t_token	*concatenate(t_token *tokens, t_token *new);
void	update_in_quote(char c, int *in_quote);
t_token_type    get_type(char c);
int     type_diff(char c1, char c2);
void    print_tokens(t_token *tokens);
void	free_tokens(t_token **tokens);

/* parse tokens */
t_list  *parse_tokens(t_token *tokens);
t_list  *gen_exec_list(t_token *tokens);
t_list  *parse_cmd(t_token **tokens);
bool    parse_redir(char *file, t_token **redir_list, t_token **tokens);
bool    parse_cmd_and_arg(t_list **cmd_arg, t_token **tokens);
bool    reform_as_cmd_arr(t_list *cmd_arg, t_proc *cmd);
void    free_cmd_arg_list(t_list **cmd_arg);
void    print_exec_list(t_list *exec_list);
void    free_cmd_arr(char ***p_cmd_arr);
void    free_exec_list(t_list **exec_list);

/* main process */
// void	shell_routine(t_env *env);
t_token	*check_syntax_and_tokenize(char *line);
int		empty_line(char *line);

#endif