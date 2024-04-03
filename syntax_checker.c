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

int		syntax_error(char *line)
{
	if (has_unclosed_quote(line))
	{
		ft_putstr_fd("Syntax error: unclosed quote\n", STDERR_FILENO);
		return (1);
	}
	if (has_invalid_redir(line))
	{
		ft_putstr_fd("Syntax error: invalid redirection\n", STDERR_FILENO);
		return (1);
	}
	if (has_logical_oparator(line))
	{
		ft_putstr_fd("Syntax error: && and || not supported\n", STDERR_FILENO);
		return (1);
	}
	if (has_misplaced_oparator(line))
	{
		ft_putstr_fd("Syntax error: misplaced oparator\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int		has_unclosed_quote(char *line)
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

int		has_invalid_redir(char *line)
{
	char	redir_type;

	redir_type = 0;
	while (*line)
	{
		while (*line && !ft_strchr("<>\'\"", *line))
			line++;
		if (*line == '\0')
			return (0);
		else if (*line == '\'' || *line == '\"')
			skip_quoted(&line);
		else // if (*line == '<' || *line == '>')
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
		line++;
	}
	return (0);
}

int		has_misplaced_oparator(char *line)
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
		line++;
	}
	return (cmd_is_expected);
}

int		has_logical_oparator(char *line)
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
		line++;
	}
	return (0);
}
