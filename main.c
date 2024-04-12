/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:16:09 by ael-mank          #+#    #+#             */
/*   Updated: 2024/03/13 09:49:00 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Ex: cat < "Makefile" | grep something > outfile | wc -l < outfile

// Only with pipe '|' can a single cmd line include multiple cmds. 
// (another possible way is ';' but we don't need to handle this)

// Todo: consider the $envvar expansion
// retrieve the envp at the beginning and stock into a char **

// Next: create the pipe and execute

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

// void	shell_routine(t_env *env)
int		main(void)
{
	char	*line;
	// int		status;
	t_token	*tokens;
	t_list	*exec_list;

	while (1)
	{
		// status = 0;
		if (GNL == 0)
			line = readline("minishell> ");
		else
			line = get_next_line(0);
		if (!line)
			break ;
		if (empty_line(line))
			continue ;
		add_history(line);
		// printf("line of len %ld: %s\n", strlen(line), line);
		tokens = check_syntax_and_tokenize(line);
		exec_list = parse_tokens(tokens);
		free_exec_list(&exec_list);
		// if (!tokens)
		// 	status = 258;
		// if (!status)
		// {
		// 	exec_list = parse_tokens(&tokens);
		// 	execution_manager(exec_list, env, &status);
		// 	free_exec_list(exec_list);
		// }
		// update_env_status(env, status, "=?");
	}
	rl_clear_history();
	return (0);
}