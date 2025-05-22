/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:32:38 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/22 15:02:16 by aysadeq          ###   ########.fr       */
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
		free(tokens[i]);
		i++;
	}
	free(tokens);
}