/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_expand_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:07:08 by yrigny            #+#    #+#             */
/*   Updated: 2024/04/17 15:07:10 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool    has_expandable_dollar_str(t_list *arg, int *dollar_pos)
{
    char    *str;
    int     single_quote_open;

    str = (char *)(arg->content);
    single_quote_open = 0;
    while (*str)
    {
        if (*str == '\'')
            single_quote_open = (single_quote_open + 1) % 2;
        else if (!single_quote_open && *str == '$'
                && (ft_isalnum(*(str + 1)) || *(str + 1) == '_'))
        {
            *dollar_pos = str - (char *)arg->content;
            return (1);
        }
        str++;
    }
    return (0);
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

// go through the str, when meeting a quote:
// save the quote char (' or "), 
// continue going through the str, looking for the quote char
// when meeting the end quote, save "number of pair of quote" + 1
// loop until the str terminator \0
// malloc a new str with len = old_str_len - number of pair of quote * 2
// assemble the new str without quotes
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
