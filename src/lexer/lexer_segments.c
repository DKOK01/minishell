/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_segments.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 10:00:00 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/27 17:24:47 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

void	add_segment_to_token(t_token *token, char *value, int quote_type)
{
	t_segment	**new_segments;
	int			i;

	new_segments = malloc(sizeof(t_segment *) * (token->seg_count + 2));
	if (!new_segments)
		return ;
	i = 0;
	while (i < token->seg_count)
	{
		new_segments[i] = token->segments[i];
		i++;
	}
	new_segments[i] = create_segment(value, quote_type);
	new_segments[i + 1] = NULL;
	if (token->segments)
		free(token->segments);
	token->segments = new_segments;
	token->seg_count++;
}
