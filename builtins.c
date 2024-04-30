#include "minishell.h"

int	ft_echo(char **args)
{
	int(i) = 1;
	int(n) = 0;
	while (ft_strncmp("-n", args[i], 2) == 0)
	{
		n = 1;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (!n)
		printf("\n");
	return (0);
}

void	edit_env_value(t_list *env, char *name, char *new_value)
{
	t_env	*env_var;

	while (env)
	{
		env_var = (t_env *)env->content;
		if (strcmp(env_var->name, name) == 0)
		{
			free(env_var->value);
			env_var->value = strdup(new_value);
			if (!env_var->value)
				return ;
			break ;
		}
		env = env->next;
	}
}

int	ft_cd(char **args, t_list *env)
{
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (args[1] == NULL)
	{
		edit_env_value(env, "OLDPWD", old_pwd);
		free(old_pwd);
		if (chdir("/home") != 0)
			perror("ft_cd");
	}
	else
	{
		edit_env_value(env, "OLDPWD", old_pwd);
		free(old_pwd);
		if (chdir(args[1]) != 0)
			perror("ft_cd");
	}
	return (0);
}

int	err(const char *msg, int ret)
{
	if (msg)
		ft_printf("%s\n", msg);
	return (ret);
}

int	ft_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		return (err("Error getting current directory", 1));
	ft_printf("%s\n", path);
	free(path);
	return (0);
}

int	is_name_valid(const char *name)
{
	if (!name)
		return (0);
	int(i) = 0;
	while (name[i])
	{
		if (!ft_isalnum(name[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_export(char **args, t_list *env)
{
	t_env	*env_var;
	t_list	*env_var_node;
	char	**name_value;

	name_value = ft_split(args[1], '=');
	if (!is_name_valid(name_value[0]) || !name_value[1])
	{
		free_str_arr(&name_value);
		return (1);
	}
	env_var = malloc(sizeof(t_env));
	if (!env_var)
		return (1);
	env_var->name = ft_strdup(name_value[0]);
	if (!env_var->name)
		return (1);
	env_var->value = ft_strdup(name_value[1]);
	if (!env_var->value)
		return (1);
	free_str_arr(&name_value);
	env_var_node = ft_lstnew(env_var);
	if (!env_var_node)
		return (1);
	ft_lstadd_back(&env, env_var_node);
	return (0);
}

void	free_and_relink(t_list *prev, t_list *current)
{
	t_env	*env_var;

	prev->next = current->next;
	env_var = (t_env *)current->content;
	free(env_var->name);
	free(env_var->value);
	free(current->content);
	free(current);
}

int	ft_unset(char **args, t_list *env)
{
	t_list	*prev;
	t_list	*current;
	t_env	*env_var;

	prev = NULL;
	current = env;
	while (current)
	{
		env_var = (t_env *)current->content;
		if (strcmp(env_var->name, args[1]) == 0) // forbidden function
		{
			if (prev)
				free_and_relink(prev, current);
			else
				env = current->next;
			break ;
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

int	print_env(t_list *env)
{
	t_list	*tmp;

	tmp = env;
	while (tmp)
	{
		ft_printf("%s=%s\n", ((t_env *)tmp->content)->name,
			((t_env *)tmp->content)->value);
		tmp = tmp->next;
	}
	return (0);
}

void	ft_exit(t_ms *shell)
{
	(void)shell;
	ft_printf("exit\n");
	free_cmd_list();
	free_env();
	rl_clear_history();
	exit(0);
}
