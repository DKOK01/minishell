/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:31:47 by aysadeq           #+#    #+#             */
/*   Updated: 2025/06/29 20:04:13 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_redir_token(char *input, int *i, int *j, t_token **tokens)
{
	char	*word;

	word = ft_substr(input, *i, 2);
	tokens[(*j)++] = make_token(word, 0);
	*i += 2;
}

static void	handle_single_token(char *input, int *i, int *j, t_token **tokens)
{
	char	*word;

	word = ft_substr(input, *i, 1);
	tokens[(*j)++] = make_token(word, 0);
	(*i)++;
}

static t_token	**init_tokens(char *input)
{
	t_token	**tokens;
	int		k;

	tokens = malloc(sizeof(t_token *) * (ft_strlen(input) + 1));
	if (!tokens)
		return (NULL);
	k = 0;
	while (k <= (int)ft_strlen(input))
		tokens[k++] = NULL;
	return (tokens);
}

static int	process_tokens(char *input, t_token **tokens)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (input[i])
	{
		skip_whitespace(input, &i);
		if (input[i] == '\0')
			break ;
		if ((input[i] == '>' || input[i] == '<') && input[i + 1] == input[i])
			handle_redir_token(input, &i, &j, tokens);
		else if (input[i] == '>' || input[i] == '<' || input[i] == '|')
			handle_single_token(input, &i, &j, tokens);
		else
		{
			if (!handle_word_with_segments(input, &i, &j, tokens))
				return (0);
		}
	}
	tokens[j] = NULL;
	return (1);
}

t_token	**tokenize_input(char *input)
{
	t_token	**tokens;

	if (!input)
		return (NULL);
	tokens = init_tokens(input);
	if (!tokens)
		return (NULL);
	if (!process_tokens(input, tokens))
		return (free_tokens(tokens), NULL);
	return (tokens);
}
