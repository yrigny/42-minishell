#include "minishell.h"

bool	init_env(char **envp, t_ms *ms)
{
	int	i;
	int	ret;

	i = -1;
	while (envp[++i])
	{
		ret = !add_env_var(envp[i], &ms->env);
	}
	return (ret);
}

int	add_env_var(char *str, t_list **env)
{
	int		i;
	t_env	*env_var;
	t_list	*env_node;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	env_var = malloc(sizeof(t_env));
	if (!env_var)
		return (1);
	env_var->name = ft_substr(str, 0, i);
	if (!env_var->name)
		return (1);
	env_var->value = ft_strdup(&str[i + 1]);
	if (!env_var->value)
		return (1);
	env_node = ft_lstnew(env_var);
	if (!env_var)
		return (1);
	ft_lstadd_back(env, env_node);
	return (0);
}

void	free_env(void)
{
	t_list	*env;
	t_list	*tmp;

	env = get_ms()->env;
	while (env)
	{
		tmp = env;
		env = env->next;
		free(((t_env *)tmp->content)->name);
		free(((t_env *)tmp->content)->value);
		free(tmp->content);
		free(tmp);
	}
}

void	shell_routine(void)
{
	char	*line;
	t_token	*tokens;

	while (1)
	{
		if (GNL == 0)
			line = readline("minishell> ");
		else
			line = get_next_line(0);
		if (!line)
			break ;
		if (empty_line(line))
			continue ;
		add_history(line);
		tokens = tokenize_and_check_syntax(line);
		if (!tokens)
			continue ;
		parse_token_into_cmds(tokens);
		exec_manager();
		free_cmd_list();
	}
	rl_clear_history();
}
