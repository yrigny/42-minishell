/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 20:10:40 by yrigny            #+#    #+#             */
/*   Updated: 2024/03/30 20:10:43 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_isspace(char c)
{
	if (c == ' ' || (c >= '\t' && c <= '\r'))
		return (1);
	return (0);
}

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

void	skip_space(char **line)
{
	while (**line && ft_isspace(**line))
		(*line)++;
}

void	skip_quoted(char **line)
{
	char	quote_type;

	quote_type = **line;
	(*line)++;
	while (**line && **line != quote_type)
		(*line)++;
	if (**line == quote_type)
		(*line)++;
}

void	update_open_quote(char **line, int *single_quote, int *double_quote)
{
	char	c;

	c = **line;
	if (!(c == '\'' || c == '\"'))
		return ;
	if (c == '\'')
		*single_quote = (*single_quote + 1) % 2;
	else if (c == '\"')
		*double_quote = (*double_quote + 1) % 2;
	(*line)++;
}
