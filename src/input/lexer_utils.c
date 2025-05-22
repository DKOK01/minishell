/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 09:50:11 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/22 09:50:30 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_redir_token(char *input, int *i, int *j, t_token **tokens)
{
	char	*word;

	word = ft_substr(input, *i, 2);
	tokens[(*j)++] = make_token(word, 0);
	*i += 2;
}

void	handle_single_token(char *input, int *i, int *j, t_token **tokens)
{
	char	*word;

	word = ft_substr(input, *i, 1);
	tokens[(*j)++] = make_token(word, 0);
	(*i)++;
}

void	handle_quote_token(char *input, int *i, int *j, t_token **tokens)
{
	int		q;
	char	*word;

	q = 0;
	word = extract_quoted(input, i, &q);
	tokens[(*j)++] = make_token(word, q);
}

void	handle_word_token(char *input, int *i, int *j, t_token **tokens)
{
	char	*word;

	word = extract_word(input, i);
	tokens[(*j)++] = make_token(word, 0);
}

void	skip_spaces(char *input, int *i)
{
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
}
