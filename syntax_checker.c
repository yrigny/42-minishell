/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 20:10:19 by yrigny            #+#    #+#             */
/*   Updated: 2024/03/30 20:10:25 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*check_syntax_and_tokenize(char *line)
{
	char	*trimmed_line;
	t_token	*tokens;

	trimmed_line = trim_line(line);
	tokens = NULL;
	free(line);
	if (!trimmed_line)
		return (NULL);
	if (syntax_error(trimmed_line))
	{
		free(trimmed_line);
		return (NULL);
	}
	tokens = tokenize(trimmed_line);
	free(trimmed_line);
	return (tokens);
}

bool	has_unclosed_quote(char *line)
{
	char	quote_type;

	quote_type = 0;
	while (*line)
	{
		if (*line == '\'' || *line == '\"')
		{
			quote_type = *line;
			line++;
			while (*line && *line != quote_type)
				line++;
			if (*line == quote_type)
				quote_type = 0;
			else
				return (1);
		}
		line++;
	}
	return (0);
}

bool	has_invalid_redir(char *line)
{
	char	redir_type;

	redir_type = 0;
	while (*line)
	{
		while (*line && !ft_strchr("<>\'\"", *line))
			line++;
		if (*line == '\'' || *line == '\"')
			skip_quoted(&line);
		else if (*line == '<' || *line == '>')
		{
			redir_type = *line;
			if (*(line + 1) == redir_type)
				line++;
			line++;
			while (*line && ft_isspace(*line))
		        line++;
			if (*line == '\0' || ft_strchr("<>&|;", *line))
				return (1);
		}
		if (*line)
			line++;
	}
	return (0);
}

bool	has_misplaced_oparator(char *line)
{
	int	single_quote_open;
	int	double_quote_open;
	int cmd_is_expected;

	single_quote_open = 0;
	double_quote_open = 0;
	cmd_is_expected = 0;
	if (*line == '|' || *line == '&')
		return (1);
	while (*line)
	{
		update_open_quote(&line, &single_quote_open, &double_quote_open);
		if (*line && (*line == '|' || *line == '&')
                && !single_quote_open && !double_quote_open)
		{
			if (cmd_is_expected)
				return (1);
			cmd_is_expected = 1;
		}
		else if (!ft_isspace(*line))
			cmd_is_expected = 0;
		if (*line)
			line++;
	}
	return (cmd_is_expected);
}

bool	has_logical_oparator(char *line)
{
	int	single_quote_open;
	int	double_quote_open;

	single_quote_open = 0;
	double_quote_open = 0;
	while (*line)
	{
		update_open_quote(&line, &single_quote_open, &double_quote_open);
		if (*line && !single_quote_open && !double_quote_open)
		{
			if ((*line == '|' && *(line + 1) == '|')
					 || (*line == '&' && *(line + 1) == '&'))
			return (1);
		}
		if (*line)
			line++;
	}
	return (0);
}
