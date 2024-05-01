#include "minishell.h"

int	ft_export(char **args, t_list *env)
{
	int		exit_code;
	int		i;

	exit_code = 0;
	if (!args[1])
		ft_putstr_fd("export: usage: export name=value\n", 2);
	i = 0;
    while (args[++i])
	{
        if (!ft_strchr(args[i], '=') && has_valid_name(args[i]))
            continue ;
        if (!has_valid_name(args[i]))
		{
			export_invalid_msg(args[i]);
			exit_code = 1;
            continue ;
		}
		add_or_edit_env_var(args[i], &env);
	}
	return (exit_code);
}

bool	has_valid_name(const char *str)
{
    int i;

	if (!str || !(*str) || *str == '=')
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void    add_or_edit_env_var(char *str, t_list **env_list)
{
    int     i;
    t_list  *curr;
    t_env   *env_var;

    i = 0;
    curr = *env_list;
	while (str[i] && str[i] != '=')
		i++;
    while (curr)
    {
        env_var = (t_env *)curr->content;
        if (!ft_strncmp(env_var->name, str, i)
                && ft_strlen(env_var->name) == (size_t)i)
        {
            free(env_var->value);
            env_var->value = ft_strdup(&str[i + 1]);
            return ;
        }
        curr = curr->next;
    }
    if (!curr)
        add_env_var(str, env_list);
}

void	export_invalid_msg(char *arg)
{
	char	*tmp;
	char	*final;

	tmp = ft_strjoin("minishell: export: `", arg);
	final = ft_strjoin(tmp, "': not a valid identifier\n");
	ft_putstr_fd(final, 2);
    free(tmp);
    free(final);
}
