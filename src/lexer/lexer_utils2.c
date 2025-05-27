/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:32:38 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/27 17:19:34 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	append_quoted_part(char **result, char *input, int *i)
{
	char	*quoted;
	char	*temp;
	int		qtype;

	quoted = extract_quoted(input, i, &qtype);
	temp = ft_strjoin(*result, quoted);
	free(*result);
	free(quoted);
	*result = temp;
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
