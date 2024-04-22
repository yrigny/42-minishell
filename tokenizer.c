#include "minishell.h"

t_token *tokenize_and_check_syntax(char *line)
{
    char	*trimmed_line;
	t_token	*tokens;

	trimmed_line = trim_line(line);
	free(line);
	if (!trimmed_line)
		return (NULL);
    tokens = tokenize(trimmed_line);
    free(trimmed_line);
    if (!tokens)
		return (NULL);
    if (syntax_error(tokens))
	{
		free_tokens(&tokens);
		return (NULL);
	}
    return (tokens);
}

t_token *tokenize(char *line)
{
    t_token	        *tokens;
	char	        *start;
    char            *end;
    t_token_type    type;

	tokens = NULL;
	while (*line)
	{
		start = line;
        type = get_token_type(start);
        end = find_end_of_token(start, type);
        if (ft_strchr("|&;()<>", *start))
            tokens = concatenate(tokens, new_metacharacter(start, type));
        else
            tokens = concatenate(tokens, new_word(start, end));
        line = end;
        while (*line && ft_isspace(*line))
		    line++;
	}
	// print_tokens(tokens);
	return (tokens);
}

t_token	*new_metacharacter(char *start, t_token_type type)
{
    if (type == TOKEN_PIPE)
	    return(new_token(type, "|"));
    else if (type == TOKEN_OR)
        return(new_token(type, "||"));
    else if (type == TOKEN_AND)
        return(new_token(type, "&&"));
    else if (type == TOKEN_PRT_L)
        return(new_token(type, "("));
    else if (type == TOKEN_PRT_R)
        return(new_token(type, ")"));
	else if (type == TOKEN_REDIR_IN)
		return(new_token(type, "<"));
	else if (type == TOKEN_REDIR_HEREDOC)
		return(new_token(type, "<<"));
	else if (type == TOKEN_REDIR_OUT)
		return(new_token(type, ">"));
	else if (type == TOKEN_REDIR_APPEND)
		return(new_token(type, ">>"));
    else if (type == TOKEN_UNACCEPTED && *start == '&')
        return (new_token(type, "&"));
    else if (type == TOKEN_UNACCEPTED && *start == ';')
        return (new_token(type, ";"));
    else
        return (NULL);
}

t_token	*new_word(char *start, char *end)
{
	char	*word;
	t_token	*new;

	word = NULL;
	new = NULL;
	word = malloc(end - start + 1);
	if (!word)
		return (NULL);
	ft_strlcpy(word, start, end - start + 1);
	new = new_token(TOKEN_WORD, word);
	free(word);
	return (new);
}

t_token	*new_token(t_token_type type, char *value)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->type = type;
	new->value = ft_strdup(value);
	if (!new->value)
	{
		free(new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}
