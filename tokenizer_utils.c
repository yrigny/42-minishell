/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 16:12:09 by yrigny            #+#    #+#             */
/*   Updated: 2024/04/01 16:12:11 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_in_quote(char c, int *in_quote)
{
	if (*in_quote == 0 && c == '\'')
		*in_quote = 1;
	else if (*in_quote == 0 && c == '\"')
		*in_quote = 2;
	else if (*in_quote == 1 && c == '\'')
		*in_quote = 0;
	else if (*in_quote == 2 && c == '\"')
		*in_quote = 0;
}

t_token_type    get_type(char c)
{
    if (c == '|')
        return (TOKEN_PIPE);
    else if (c == '<')
        return (TOKEN_REDIR_IN);
    else if (c == '>')
        return (TOKEN_REDIR_OUT);
    else
        return (TOKEN_WORD);
}

int type_diff(char c1, char c2)
{
    if (get_type(c1) != get_type(c2))
        return (1);
    return (0);
}

void    print_tokens(t_token *tokens)
{
    while (tokens)
    {
        printf("type: %d\tvalue: [%s]\n", tokens->type, tokens->value);
        tokens = tokens->next;
    }
}

void	free_tokens(t_token **tokens)
{
	t_token	*temp;

    temp = NULL;
	while (*tokens)
	{
		temp = (*tokens)->next;
		free((*tokens)->value);
		free(*tokens);
		*tokens = temp;
	}
}
