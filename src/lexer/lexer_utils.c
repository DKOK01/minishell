/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 09:50:11 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/30 09:24:18 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*make_token(char *value, int quoted)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->quoted = quoted;
	token->segments = NULL;
	token->seg_count = 0;
	return (token);
}

void	skip_spaces(char *input, int *i)
{
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
}

void	free_tokens(t_token **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->value)
			free(tokens[i]->value);
		if (tokens[i]->segments)
			free_segments(tokens[i]->segments);
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
