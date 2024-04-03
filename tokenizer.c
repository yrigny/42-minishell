/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 15:47:21 by yrigny            #+#    #+#             */
/*   Updated: 2024/04/01 15:47:24 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_token	*new_word(char *start, char **end)
{
	char	*word;
	t_token	*new;

	word = NULL;
	new = NULL;
	word = malloc(*end - start + 1);
	if (!word)
		return (NULL);
	ft_strlcpy(word, start, *end - start + 1);
	new = new_token(TOKEN_WORD, word);
	free(word);
	while (**end && ft_isspace(**end))
		(*end)++;
	return (new);
}

t_token	*new_metacharacter(char *start, char **end)
{
	t_token	*new;

	new = NULL;
	if (*start == '<' && (*end - start == 1))
		new = new_token(TOKEN_REDIR_IN, "<");
	if (*start == '<' && (*end - start == 2))
		new = new_token(TOKEN_REDIR_HEREDOC, "<<");
	if (*start == '>' && (*end - start == 1))
		new = new_token(TOKEN_REDIR_OUT, ">");
	if (*start == '>' && (*end - start == 2))
		new = new_token(TOKEN_REDIR_APPEND, ">>");
	if (*start == '|')
	    new = new_token(TOKEN_PIPE, "|");
	while (**end && ft_isspace(**end))
		(*end)++;
	return (new);
}

t_token	*concatenate(t_token *tokens, t_token *new)
{
	t_token	*tail;

	if (!new)
	{
		free_tokens(&tokens);
		ft_putstr_fd("Tokenization failed - malloc error.", 2);
		return (NULL);
	}
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

t_token	*tokenize(char *line)
{
	t_token	*tokens;
	char	*start;
	int		in_quote;

	tokens = NULL;
	in_quote = 0;
	while (*line)
	{
		start = line;
		while (*line)
		{
			update_in_quote(*line, &in_quote);
			line++;
			if (!in_quote && (ft_isspace(*line) || type_diff(*start, *line)))
				break ;
		}
		if (*start && ft_strchr("<>|", *start))
			tokens = concatenate(tokens, new_metacharacter(start, &line));
		else
			tokens = concatenate(tokens, new_word(start, &line));
		if (!tokens)
			break ;
	}
	return (tokens);
}
