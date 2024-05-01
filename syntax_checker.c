#include "minishell.h"

bool    syntax_error(t_token *tokens)
{
    int open_prt;

    open_prt = 0;
    if (tokens->type == TOKEN_PIPE)
        return (syntax_error_msg(tokens->value));
    while (tokens)
    {
        if (tokens->type >= TOKEN_UNACCEPTED)
            return (syntax_error_msg(tokens->value));
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
    if (!next && !(curr->type == TOKEN_WORD))
        return (syntax_error_msg("newline"));
    if (!next && curr->type == TOKEN_WORD)
        return (0);
    if (curr->type == TOKEN_PIPE && next->type == TOKEN_PIPE)
        return (syntax_error_msg(next->value));
    if (curr->type >= TOKEN_REDIR_IN && curr->type <= TOKEN_REDIR_APPEND
            && next->type != TOKEN_WORD)
        return (syntax_error_msg(next->value));
    return (0);
}
