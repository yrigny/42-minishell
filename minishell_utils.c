/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:56:01 by yrigny            #+#    #+#             */
/*   Updated: 2024/04/12 16:56:03 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ms	*get_ms(void)
{
	static t_ms	minishell;

	return (&minishell);
}

// t_list	*get_cmds(void)
// {
// 	t_ms	*ms;

// 	ms = get_ms();
// 	return (ms->cmds);
// }

int		empty_line(char *line)
{
	char	*temp;

	temp = line;
	while (*temp && ft_isspace(*temp))
		temp++;
	if (*temp == '\0')
	{
		free(line);
		return (1);
	}
	return (0);
}

