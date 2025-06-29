/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_status.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 12:00:00 by aysadeq           #+#    #+#             */
/*   Updated: 2025/06/29 20:05:37 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	check_all_single_quoted(t_token *token)
{
	int	i;

	i = 0;
	while (i < token->seg_count)
	{
		if (token->segments[i]->quote_type != 1)
			return (0);
		i++;
	}
	return (1);
}

void	set_token_quoted_status(t_token *token, int has_unquoted_segments)
{
	if (token->seg_count > 0 && !has_unquoted_segments)
	{
		if (check_all_single_quoted(token))
			token->quoted = 1;
	}
}

void	cleanup_failed_token(t_token *token, char *full_value)
{
	free(full_value);
	free_segments(token->segments);
	free(token);
}

void	skip_whitespace(char *input, int *i)
{
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
}
