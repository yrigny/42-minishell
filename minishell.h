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

# define SUCCESS 1
# define FAILURE 0
# ifndef MAX_PIPE
#  define MAX_PIPE 1024
# endif

typedef enum e_token_type
{
    TOKEN_WORD,         // cmd and args
    TOKEN_PIPE,         // '|'
    TOKEN_OR,           // '||'
    TOKEN_AND,          // '&&'
    TOKEN_PRT_L,        // '('
    TOKEN_PRT_R,        // ')'
    TOKEN_REDIR_IN,     // '<'
    TOKEN_REDIR_HEREDOC,// '<<'
    TOKEN_REDIR_OUT,    // '>'
    TOKEN_REDIR_APPEND, // '>>'
    TOKEN_UNACCEPTED,   // '&' ';'
}   t_token_type;

typedef struct s_token
{
    t_token_type    type;
    char            *value;
    struct s_token  *next;
}   t_token;

typedef struct s_cmd
{
    char    **cmd_arr; // filled by token parser
    char    *fullpath; // filled by pre-expand
    t_token *redir_in; // filled by token parser
    t_token *redir_out; // filled by token parser
    int     fd_in; // to do in pipe parser
    int     fd_out; // to do in pipe parser
}   t_cmd;

typedef struct s_env
{
    char    *name;
    char    *value;
}   t_env;

typedef struct s_pipe
{
    int     fd[2];
    pid_t   pid;
}   t_pipe;

typedef struct s_ms
{
    char    *curr_dir; // why this?
    t_list  *env;
    t_list  *cmds;
    t_pipe  pipe[MAX_PIPE];
    int     last_exit;
    pid_t   last_pipe_pid; // what's this?
}   t_ms;


/* main process */
t_ms	*get_ms(void);
bool	init_env(char **envp, t_ms *ms);
void	shell_routine(void);
void	free_env(t_list *env);
int		empty_line(char *line);

/* tokenization */
t_token *tokenize_and_check_syntax(char *line);
char	*trim_line(char *str);
t_token *tokenize(char *line);
char    *find_end_of_token(char *s, t_token_type type);
t_token	*new_metacharacter(char *start, t_token_type type);
t_token	*new_word(char *start, char *end);
t_token	*new_token(t_token_type type, char *value);
t_token	*concatenate(t_token *tokens, t_token *new);
t_token_type    get_token_type(char *s);
void	skip_if_quoted(char **line);
void    print_tokens(t_token *tokens);
void	free_tokens(t_token **tokens);

/* syntax check */
bool    syntax_error(t_token *tokens);
bool    update_parenthesis(t_token_type type, int *open_prt);
bool    has_unclosed_quote(char *str);
bool    wrong_next_token_type(t_token *curr, t_token *next);
bool    syntax_error_msg(char *s);

/* parse tokens */


/* pre-expand */


#endif