/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:31:47 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/30 09:22:32 by aysadeq          ###   ########.fr       */
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

static void	handle_token(char *input, int *i, int *j, t_token **tokens)
{
	if ((input[*i] == '>' || input[*i] == '<') && input[*i + 1] == input[*i])
		handle_redir_token(input, i, j, tokens);
	else if (input[*i] == '>' || input[*i] == '<' || input[*i] == '|')
		handle_single_token(input, i, j, tokens);
	else
		handle_word_with_segments(input, i, j, tokens);
}

t_token	**tokenize_input(char *input)
{
	t_token	**tokens;
	int		i;
	int		j;

	i = 0;
	j = 0;
	tokens = malloc(sizeof(t_token *) * (ft_strlen(input) + 1));
	if (!tokens)
		return (NULL);
	while (input[i])
	{
		skip_spaces(input, &i);
		if (input[i] == '\0')
			break ;
		handle_token(input, &i, &j, tokens);
	}
	tokens[j] = NULL;
	return (tokens);
}
