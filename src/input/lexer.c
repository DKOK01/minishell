/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:31:47 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/20 13:57:44 by aysadeq          ###   ########.fr       */
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
	return (token);
}

char	*extract_word(char *input, int *i)
{
	int		start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '\"' && input[*i] != '\'' && input[*i] != '|' && input[*i] != '>' && input[*i] != '<')
		(*i)++;
	return (ft_substr(input, start, *i - start));
}

char	*extract_quoted(char *s, int *i, int *qtype)
{
	char	quote;
	int		start;

	quote = s[*i];
	start = ++(*i);
	if (quote == '\'')
		*qtype = 1;
	else
		*qtype = 2;
	while (s[*i] && s[*i] != quote)
		(*i)++;
	if (s[*i] == quote)
		(*i)++;
	return (ft_substr(s, start, *i - start - 1));
}

t_token	**tokenize_input(char *input)
{
	t_token	**tokens;
	int		i;
	int		j;
	int 	q;
	char	*word;

	i = 0;
	j = 0;
	tokens = malloc(sizeof(t_token *) * (ft_strlen(input) + 1));
	if (!tokens)
		return (NULL);
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		if (input[i] == '\0')
			break;
		q = 0;
		if ((input[i] == '>' || input[i] == '<') && input[i + 1] == input[i])
		{
			word = ft_substr(input, i, 2);
			tokens[j++] = make_token(word, 0);
			i+= 2;
		}
		else if (input[i] == '>' || input[i] == '<' || input[i] == '|')
		{
			word = ft_substr(input, i, 1);
			tokens[j++] = make_token(word, 0);
			i++;
		}
		else if (input[i] == '\'' || input[i] == '\"')
		{
			word = extract_quoted(input, &i, &q);
			tokens[j++] = make_token(word, q);
		}
		else
		{
			word = extract_word(input, &i);
			tokens[j++] = make_token(word, 0);
		}
	}
	tokens[j] = NULL;
	return (tokens);
}

