#include "minishell.h"

char	*trim_line(char *str)
{
    int     i_start;
    int     i_end;
	char	*trimmed;

    if (!str)
        return (NULL);
    if (ft_strlen(str) == 0)
        return (ft_strdup(str));
    i_start = 0;
    i_end = ft_strlen(str) - 1;
    while (str[i_start] && ft_isspace(str[i_start]))
        i_start++;
    while (i_end && ft_isspace(str[i_end]))
        i_end--;
    if (i_start > i_end) // str has only whitespaces
        trimmed = malloc(1);
    else
        trimmed = malloc(i_end - i_start + 2);
    if (!trimmed)
        return (NULL);
    if (i_start > i_end)
        trimmed[0] = '\0';
    else
        ft_strlcpy(trimmed, &str[i_start], i_end - i_start + 2);
    return (trimmed);
}

t_token_type    get_token_type(char *s)
{
    if (*s == '|' && *(s + 1) == '|')
        return (TOKEN_OR);
    else if (*s == '|' && *(s + 1) != '|')
        return (TOKEN_PIPE);
    else if (*s == '&' && *(s + 1) == '&')
        return (TOKEN_AND);
    else if (*s == '(')
        return (TOKEN_PRT_L);
    else if (*s == ')')
        return (TOKEN_PRT_R);
    else if (*s == '<' && *(s + 1) == '<')
        return (TOKEN_REDIR_HEREDOC);
    else if (*s == '<' && *(s + 1) != '<')
        return (TOKEN_REDIR_IN);
    else if (*s == '>' && *(s + 1) == '>')
        return (TOKEN_REDIR_APPEND);
    else if (*s == '>' && *(s + 1) != '>')
        return (TOKEN_REDIR_OUT);
    else if (*s == '&' && *(s + 1) != '&')
        return (TOKEN_UNACCEPTED);
    else if (*s == ';')
        return (TOKEN_UNACCEPTED);
    else
        return (TOKEN_WORD);
}

char    *find_end_of_token(char *s, t_token_type type)
{
    if (type > TOKEN_WORD)
    {
        if (type == TOKEN_REDIR_HEREDOC || type == TOKEN_REDIR_APPEND
            || type == TOKEN_AND || type == TOKEN_OR)
            return (s + 2);
        else
            return (s + 1);
    }
    else
    {
        while (*s)
        {
            skip_if_quoted(&s);
            if (!(*s) || ft_strchr(" \t\n|&;()<>", *s))
                break ;
            s++;
        }
        return (s);
    }
}

void	skip_if_quoted(char **line)
{
	char	quote_type;

    if (**line == '\'' || **line == '\"')
    {
        quote_type = **line;
        (*line)++;
        while (**line && **line != quote_type)
            (*line)++;
        if (**line == quote_type)
            (*line)++;
    }
}

t_token	*concatenate(t_token *tokens, t_token *new)
{
	t_token	*tail;

	if (!new)
		return (tokens);
	if (!tokens)
		tokens = new;
	else
	{
		tail = tokens;
		while (tail->next)
			tail = tail->next;
		tail->next = new;
	}
	return (tokens);
}

void    print_tokens(t_token *tokens)
{
    while (tokens)
    {
        printf("type: %d\tvalue: [%s]\n", tokens->type, tokens->value);
        tokens = tokens->next;
    }
}

void	free_tokens(t_token **tokens)
{
	t_token	*temp;

    temp = NULL;
	while (*tokens)
	{
		temp = (*tokens)->next;
		free((*tokens)->value);
		free(*tokens);
		*tokens = temp;
	}
}