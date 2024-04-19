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

	while (*line)
	{
		quote_type = 0;
		if (*line == '\'' || *line == '\"')
		{
			quote_type = *line;
			line += 1;
			while (*line && *line != quote_type)
				line++;
			if (*line == '\0')
			{
				syntax_error_pos(line);
				return (1);
			}
		}
		line++;
	}
	return (0);
}

bool	has_unclosed_parenthesis(char *line)
{
	int	parenthesis_open;

	while (*line)
	{
		parenthesis_open = 0;
		if (*line == '\'' || *line == '\"')
			skip_quoted(&line);
		if (*line == ')')
		{
			syntax_error_pos(line);
			return (1);
		}
		if (*line == '(')
		{
			line += 1;
			while (*line && *line != ')')
				line++;
			if (*line == '\0')
			{
				syntax_error_pos(line);
				return (1);
			}
		}
		line++;
	}
	return (0);
}

bool	has_invalid_redir(char *line)
{
	while (*line)
	{
		if (*line == '\'' || *line == '\"')
			skip_quoted(&line);
		else if (*line == '<' || *line == '>')
		{
			if (*(line + 1) == *line)
				line += 1;
			line += 1;
			skip_space(&line);
			if (*line == '\0' || ft_strchr("|&;()<>", *line))
			{
				syntax_error_pos(line);
				return (1);
			}
		}
		if (*line)
			line++;
	}
	return (0);
}

// bool	has_logical_oparator(char *line)
// {
// 	while (*line)
// 	{
// 		if (*line == '\'' || *line == '\"')
// 			skip_quoted(&line);
// 		else if ((*line == '|' && *(line + 1) == '|')
// 				|| (*line == '&' && *(line + 1) == '&'))
// 			return (1);
// 		if (*line)
// 			line++;
// 	}
// 	return (0);
// }

bool	has_misplaced_oparator(char *line)
{
	if (*line == '|' || *line == '&')
		return (1);
	while (*line)
	{
		if (*line == '\'' || *line == '\"')
			skip_quoted(&line);
		else if (*line == '|' || *line == '&')
		{
			line += 1;
			skip_space(&line);
			if (*line == '\0' || ft_strchr("|&;()<>", *line))
			{
				syntax_error_pos(line);
				return (1);
			}
		}
		if (*line)
			line++;
	}
	return (0);
}
