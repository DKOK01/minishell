/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_segments_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 10:00:00 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/30 09:21:02 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*extract_quoted(char *input, int *i, int *qtype)
{
	char	quote;
	char	*result;
	char	*temp;

	result = ft_strdup("");
	quote = input[*i];
	if (quote == '\'')
		*qtype = 1;
	else
		*qtype = 2;
	(*i)++;
	while (input[*i] && input[*i] != quote)
	{
		temp = ft_strjoin_char(result, input[*i]);
		free(result);
		result = temp;
		(*i)++;
	}
	if (input[*i] == quote)
		(*i)++;
	return (result);
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
