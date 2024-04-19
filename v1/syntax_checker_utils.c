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

bool	syntax_error(char *line)
{
	if (has_unclosed_quote(line))
		return (1);
	if (has_unclosed_parenthesis(line))
		return (1);
	if (has_invalid_redir(line))
		return (1);
	// if (has_logical_oparator(line))
	// {
	// 	ft_putstr_fd("Syntax error: && and || not supported\n", STDERR_FILENO);
	// 	return (1);
	// }
	if (has_misplaced_oparator(line))
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

void	syntax_error_pos(char *pos)
{
	if (*pos == '\0')
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
	else if (*pos == '|' && *(pos + 1) == '|')
		ft_putstr_fd("minishell: syntax error near unexpected token `||'\n", 2);
	else if (*pos == '|' && *(pos + 1) != '|')
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
	else if (*pos == '&' && *(pos + 1) == '&')
		ft_putstr_fd("minishell: syntax error near unexpected token `&&'\n", 2);
	else if (*pos == '&' && *(pos + 1) != '&')
		ft_putstr_fd("minishell: syntax error near unexpected token `&'\n", 2);
	else if (*pos == ';')
		ft_putstr_fd("minishell: syntax error near unexpected token `;'\n", 2);
	else if (*pos == '(')
		ft_putstr_fd("minishell: syntax error near unexpected token `('\n", 2);
	else if (*pos == ')')
		ft_putstr_fd("minishell: syntax error near unexpected token `)'\n", 2);
	else if (*pos == '<' && *(pos + 1) == '<')
		ft_putstr_fd("minishell: syntax error near unexpected token `<<'\n", 2);
	else if (*pos == '<' && *(pos + 1) != '<')
		ft_putstr_fd("minishell: syntax error near unexpected token `<'\n", 2);
	else if (*pos == '>' && *(pos + 1) == '>')
		ft_putstr_fd("minishell: syntax error near unexpected token `>>'\n", 2);
	else if (*pos == '>' && *(pos + 1) != '>')
		ft_putstr_fd("minishell: syntax error near unexpected token ` | >'\n", 2);
}
