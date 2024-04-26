#include "minishell.h"
/*
TOKEN_WORD,         // check unclosed quote
TOKEN_PIPE,         // '|' must not be the 1st token or followed by ')' '|' '||' '&&'
TOKEN_OR,           // '||' must not be the 1st token or followed by ')' '|' '||' '&&'
TOKEN_AND,          // '&&' must not be the 1st token or followed by ')' '|' '||' '&&'
TOKEN_PRT_L,        // '(' must not be single, or followed by ')' '|' '||' '&&'
TOKEN_PRT_R,        // ')' must not be single, or followed by word token or '('
TOKEN_REDIR_IN,     // '<' must be followed by word token
TOKEN_REDIR_HEREDOC,// '<<' idem
TOKEN_REDIR_OUT,    // '>' idem
TOKEN_REDIR_APPEND, // '>>' idem
TOKEN_UNACCEPTED,   // '&' ';' should be rejected
*/

bool    syntax_error(t_token *tokens)
{
    int open_prt;

    open_prt = 0;
    if (tokens->type >= TOKEN_PIPE && tokens->type <= TOKEN_AND)
        return (syntax_error_msg(tokens->value));
    while (tokens)
    {
        if (tokens->type == TOKEN_UNACCEPTED)
            return (syntax_error_msg(tokens->value));
        if ((tokens->type == TOKEN_PRT_L || tokens->type == TOKEN_PRT_R)
                && !update_parenthesis(tokens->type, &open_prt))
            return (1);
        if (tokens->type == TOKEN_WORD && has_unclosed_quote(tokens->value))
            return (1);
        if (wrong_next_token_type(tokens, tokens->next))
            return (1);
        tokens = tokens->next;
    }
    if (open_prt > 0)
        return (syntax_error_msg("newline"));
    return (0);
}

bool    syntax_error_msg(char *s)
{
    char    *tmp;
    char    *final;

    tmp = ft_strjoin("minishell: syntax error near unexpected token `", s);
    final = ft_strjoin(tmp, "'\n");
    ft_putstr_fd(final, 2);
    free(tmp);
    free(final);
    return (1);
}

bool    update_parenthesis(t_token_type type, int *open_prt)
{
    if (*open_prt == 0 && type == TOKEN_PRT_R)
    {
        syntax_error_msg(")");
        return (0);
    }
    else if (*open_prt > 0 && type == TOKEN_PRT_R)
        *open_prt -= 1;
    else if (type == TOKEN_PRT_L)
        *open_prt += 1;
    return (1);
}

bool    has_unclosed_quote(char *str)
{
    char	quote_type;

	while (*str)
	{
		quote_type = 0;
		if (*str == '\'' || *str == '\"')
		{
			quote_type = *str;
			str += 1;
			while (*str && *str != quote_type)
				str++;
			if (*str == '\0')
                return (syntax_error_msg("newline"));
		}
		str++;
	}
	return (0);
}

bool    wrong_next_token_type(t_token *curr, t_token *next)
{
    if (!next && !(curr->type == TOKEN_WORD || curr->type == TOKEN_PRT_R))
        return (syntax_error_msg("newline"));
    if (!next && (curr->type == TOKEN_WORD || curr->type == TOKEN_PRT_R))
        return (0);
    if ((curr->type >= TOKEN_PIPE && curr->type <= TOKEN_PRT_L)
            && ((next->type >= TOKEN_PIPE && next->type <= TOKEN_AND)
            || next->type == TOKEN_PRT_R))
        return (syntax_error_msg(next->value));
    if (curr->type == TOKEN_PRT_R 
            && (next->type == TOKEN_PRT_L || next->type == TOKEN_WORD))
        return (syntax_error_msg(next->value));
    if (curr->type >= TOKEN_REDIR_IN && curr->type <= TOKEN_REDIR_APPEND
            && next->type != TOKEN_WORD)
        return (syntax_error_msg(next->value));
    return (0);
}
