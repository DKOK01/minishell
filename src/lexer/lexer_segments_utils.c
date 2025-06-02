/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_segments_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 10:00:00 by aysadeq           #+#    #+#             */
/*   Updated: 2025/06/02 10:46:51 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*make_token_with_segments(void)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = NULL;
	token->segments = NULL;
	token->seg_count = 0;
	token->quoted = 0;
	return (token);
}

t_segment	*create_segment(char *value, int quote_type)
{
	t_segment	*seg;

	seg = malloc(sizeof(t_segment));
	if (!seg)
		return (NULL);
	seg->value = ft_strdup(value);
	seg->quote_type = quote_type;
	return (seg);
}

void	free_segments(t_segment **segments)
{
	int	i;

	if (!segments)
		return ;
	i = 0;
	while (segments[i])
	{
		free(segments[i]->value);
		free(segments[i]);
		i++;
	}
	free(segments);
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
