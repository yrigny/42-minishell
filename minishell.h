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
# include <errno.h>

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
    // pid_t   last_pipe_pid; // what's this?
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

/* syntax check */
bool    syntax_error(t_token *tokens);
bool    update_parenthesis(t_token_type type, int *open_prt);
bool    has_unclosed_quote(char *str);
bool    wrong_next_token_type(t_token *curr, t_token *next);
bool    syntax_error_msg(char *s);

/* pre-expand */
void    pre_expand(t_token *tokens);
void    expand_env_var(t_token *token, int head);
char    *match_env_var(char *name, int len);
void    remove_quotes(t_token *token, char *old_str);
bool    has_expandable_dollar_str(t_token *token, int *dollar_pos);
char    *assemble_new_str(char *old_str, char *value, int head, int end);
char    *assemble_new_str2(char *old_str, int pair_of_quotes);
void    expand_fullpath(t_list *cmds);
char    **get_paths_array(void);
void    assemble_fullpath(t_cmd *cmd, char *cmd_name, char **paths);

/* parse tokens */
void    parse_token_into_cmds(t_token *tokens);
t_list  *gen_cmd_list(t_token *tokens);
t_list  *parse_cmd(t_token **tokens);
void    parse_redir(char *file, t_token **redir_list, t_token **tokens);
void    parse_cmd_and_arg(t_list **cmd_arg, t_token **tokens);
void    reform_as_cmd_arr(t_list *cmd_arg, t_cmd *cmd);
void    free_cmd_arg_list(t_list **cmd_arg);
void    free_str_arr(char ***p_str_arr);
void    free_cmd_list(void);
void	free_tokens(t_token **tokens);

/* execution */
void	exec_manager(void);
void    single_cmd_exec(t_cmd *cmd);
void    pipex(t_ms *ms, t_list *cmds);
void    fork_children(int nb_cmds, t_pipe pipe_arr[MAX_PIPE], t_list *cmds);
void	child_first(t_cmd *child, int pipe[2]);
void	child_middle(t_cmd *child, int pipe1[2], int pipe2[2]);
void	child_last(t_cmd *child, int pipe[2]);
bool    cmd_exists(t_cmd *child);
bool	cmd_is_executable(t_cmd *child);
void	execute_cmd(t_cmd *child);
void	exec_builtin(t_cmd *child);
void	catch_last_status(int *status);

/* pre-execution */
void    handle_redir_in(t_cmd *cmd);
int     receive_heredoc(char *delimiter);
void    handle_redir_out(t_cmd *cmd);

/* helper */
void    print_tokens(t_token *tokens);
void    print_cmd_list(t_list *cmd_list);
bool    is_builtin(char *cmd_name);

/* builtins */
int	    ft_echo(char **args);
void    edit_env_value(t_list *env, char *name, char *new_value);
int	    ft_cd(char **args, t_list *env);
int	    err(const char *msg, int ret);
int	    ft_pwd(void);
int     is_name_valid(const char *name);
void	ft_export(char **args, t_list *env);
void    free_and_relink(t_list *prev, t_list *current);
void    ft_unset(char **args, t_list *env);
void	print_env(t_list *env);
void	ft_exit(t_ms *shell);

#endif