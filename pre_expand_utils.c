#include "minishell.h"

bool    has_expandable_dollar_str(t_token *token, int *dollar_pos)
{
    char    *str;
    int     single_quote_open;

    str = token->value;
    single_quote_open = 0;
    while (*str)
    {
        if (*str == '\'')
            single_quote_open = (single_quote_open + 1) % 2;
        else if (!single_quote_open && *str == '$'
                && (ft_isalnum(*(str + 1)) || *(str + 1) == '_'))
        {
            *dollar_pos = str - token->value;
            return (1);
        }
        str++;
    }
    return (0);
}

char    *assemble_new_str(char *old_str, char *value, int head, int end)
{
    int     value_len;
    int     tail_len;
    int     total_len;
    char    *new_str;

    value_len = ft_strlen(value);
    tail_len = ft_strlen(old_str) - (end + 1);
    total_len = ft_strlen(old_str) - (end - head + 1) + value_len;
    new_str = malloc(total_len + 1);
    if (!new_str)
        return (NULL);
    // copy head bytes before dollar str
    ft_strlcpy(new_str, old_str, head + 1);
    // copy env_var value string
    ft_strlcpy(new_str + head, value, value_len + 1);
    // copy tail bytes after dollar str
    ft_strlcpy(new_str + head + value_len, old_str + end + 1, tail_len + 1);
    return (new_str);
}

char    *assemble_new_str2(char *old_str, int pair_of_quotes)
{
    int     new_str_len;
    char    *new_str;
    char    quote;
    int     i;

    new_str_len = ft_strlen(old_str) - 2 * pair_of_quotes;
    new_str = malloc(new_str_len + 1);
    if (!new_str)
        return (NULL);
    i = 0;
    while (*old_str)
    {
        if (*old_str == '\'' || *old_str == '\"')
        {
            quote = *old_str;
            while (*(++old_str) != quote)
                new_str[i++] = *old_str;
        }
        else
            new_str[i++] = *old_str;
        old_str++;
    }
    new_str[i] = '\0';
    return (new_str);
}

char    **get_paths_array(void)
{
    t_list  *env;
    char    **paths;

    env = get_ms()->env;
    while (env && ft_strncmp(((t_env *)env->content)->name, "PATH", 5))
        env = env->next;
    if (env && !ft_strncmp(((t_env *)env->content)->name, "PATH", 5))
        paths = ft_split(((t_env *)env->content)->value, ':');
    else
        paths = NULL;
    return (paths);
}

void    assemble_fullpath(t_cmd *cmd, char *cmd_name, char **paths)
{
    char    *tmp_cmd_path;
    int     i;

    if (access(cmd_name, F_OK) == 0)
    {
        cmd->fullpath = ft_strdup(cmd_name);
        return ;
    }
    i = -1;
    while (!cmd->fullpath && cmd_name && paths[++i])
    {
        tmp_cmd_path = ft_strjoin(paths[i], "/");
        cmd->fullpath = ft_strjoin(tmp_cmd_path, cmd_name);
        if (access(cmd->fullpath, F_OK) != 0)
        {
            free(cmd->fullpath);
            cmd->fullpath = NULL;
        }
        free(tmp_cmd_path);
    }
}
