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

// So first cut the line by '|' to get a char ** type cmd array.
// char	**cmd_arr = ft_split(line, '|');
// cmd_arr[0] = cat < "Makefile"
// cmd_arr[1] = grep something > outfile
// cmd_arr[2] = wc -l < outfile

// Next, break up each cmd_arr element into a structured cmd table.
// typedef struct s_cmd
// {
// 	int		pipe_id;
// 	char	*path; // strdup for fullpath and strjoin for concatenated path (malloc!)
// 	char	**tokens; // split the cmd word by word excluding redirect info (malloc!)
// 	char	*in; // strdup the infile path when < or << present (malloc!)
// 	char	*out; // strdup the outfile path when > or >> present (malloc!)
// }			t_cmd;

// Now cmd_arr is expanded to a t_cmd * type array (malloc'd).
// t_cmd	*parsed_cmd_arr = malloc(sizeof(t_cmd) * 3);
// parsed_cmd_arr[0].pipe_id = 0
// parsed_cmd_arr[0].path = /usr/bin/cat
// parsed_cmd_arr[0].tokens = {cat}
// parsed_cmd_arr[0].in = Makefile
// parsed_cmd_arr[0].out = NULL
// parsed_cmd_arr[1].pipe_id = 1
// parsed_cmd_arr[1].path = /bin/grep
// parsed_cmd_arr[1].tokens = {grep, something}
// parsed_cmd_arr[1].in = NULL
// parsed_cmd_arr[1].out = outfile
// parsed_cmd_arr[2].pipe_id = 1
// parsed_cmd_arr[2].path = /usr/bin/wc
// parsed_cmd_arr[2].tokens = {wc, -l}
// parsed_cmd_arr[2].in = outfile
// parsed_cmd_arr[2].out = NULL

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
	// t_list	*exec_list;

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
		printf("line of len %ld: %s\n", strlen(line), line);
		tokens = check_syntax_and_tokenize(line);
		print_tokens(tokens);
		free_tokens(&tokens);
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