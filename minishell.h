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

typedef struct s_cmd
{
    char    **cmd_arr; // filled by token parser
    char    *fullpath; // to do later in pre-execution step
    t_token *redir_in; // filled by token parser
    t_token *redir_out; // filled by token parser
    int     fd_in; // to do in pipe parser
    int     fd_out; // to do in pipe parser
    // int  pipe[2];
}   t_cmd;

typedef struct s_env
{
    char    *name;
    char    *value;
}   t_env;

typedef struct s_ms
{
    char    *curr_dir; // why this?
    t_list  *env;
    t_list  *cmds;
    pid_t   pids[1024];
    int     last_exit;
    pid_t   last_pipe_pid; // what's this?
}   t_ms;


/* main process */
bool	init_env(char **envp, t_ms *ms);
void	shell_routine(void);
t_ms	*get_ms(void);
int		empty_line(char *line);

/* syntax check */
t_token	*check_syntax_and_tokenize(char *line);
char	*trim_line(char *str);
bool	syntax_error(char *line);
bool	has_unclosed_quote(char *line);
bool	has_invalid_redir(char *line);
bool	has_misplaced_oparator(char *line);
bool	has_logical_oparator(char *line);
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
bool    type_diff(char c1, char c2);
void	free_tokens(t_token **tokens);
void    print_tokens(t_token *tokens);

/* parse tokens */
bool    parse_tokens(t_token *tokens);
t_list  *gen_cmd_list(t_token *tokens);
t_list  *parse_cmd(t_token **tokens);
bool    parse_redir(char *file, t_token **redir_list, t_token **tokens);
bool    parse_cmd_and_arg(t_list **cmd_arg, t_token **tokens);
bool    reform_as_cmd_arr(t_list *cmd_arg, t_cmd *cmd);
void    free_cmd_arg_list(t_list **cmd_arg);
void    print_cmd_list(t_list *exec_list);
void    free_str_arr(char ***p_str_arr);
void    free_cmd_list(void);

void    pre_expand(t_list **cmd_arg, t_cmd *cmd);
void    expand_cmd_path(t_cmd *cmd, char *executable);
char    **get_paths_array(void);
bool    has_expandable_dollar_str(t_list *arg, int *dollar_pos);
void    expand_env_var(t_list **cmd_arg, int head);
char    *match_env_var(char *name, int len);
char    *assemble_new_str(char *old_str, char *value, int head, int end);

#endif