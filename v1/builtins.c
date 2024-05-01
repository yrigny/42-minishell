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
		if (!ft_strncmp(env_var->name, name, ft_strlen(name) + 1))
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

int	cd_command(t_list *env, char *path)
{
	char	*old_pwd;
	char	*pwd;
	
	old_pwd = getcwd(NULL, 0);
	edit_env_value(env, "OLDPWD", old_pwd);
	free(old_pwd);
	if (chdir(path) != 0)
	{
		perror("ft_cd");
		return (1);
	}
	pwd = getcwd(NULL, 0);
	edit_env_value(env, "PWD", pwd);
	free(pwd);
	return (0);
}

int	ft_cd(char **args, t_list *env)
{
	if (args[1] == NULL)
		return(cd_command(env, "/home"));
	else
		return(cd_command(env, args[1]));
	return (1);
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
		if (ft_strncmp(env_var->name, args[1], ft_strlen(args[1]) + 1) == 0)
		{
			if (prev)
				free_and_relink(prev, current);
			else
				env = current->next;
			return (0);
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

void	ft_exit(void)
{
	ft_printf("exit\n");
	free_cmd_list();
	free_env();
	rl_clear_history();
	exit(0);
}
