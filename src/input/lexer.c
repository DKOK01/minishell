/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:31:47 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/22 14:34:51 by aysadeq          ###   ########.fr       */
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
	char	*result;
	char	*temp;

	result = ft_strdup("");
	while (input[*i] && !ft_isspace(input[*i])
		&& input[*i] != '|' && input[*i] != '>' && input[*i] != '<')
	{
		if (input[*i] == '"' || input[*i] == '\'')
			append_quoted_part(&result, input, i);
		else
		{
			temp = ft_strjoin_char(result, input[*i]);
			free(result);
			result = temp;
			(*i)++;
		}
	}
	return (result);
}

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

static void	handle_token(char *input, int *i, int *j, t_token **tokens)
{
	if ((input[*i] == '>' || input[*i] == '<') && input[*i + 1] == input[*i])
		handle_redir_token(input, i, j, tokens);
	else if (input[*i] == '>' || input[*i] == '<' || input[*i] == '|')
		handle_single_token(input, i, j, tokens);
	else if (input[*i] == '\'' || input[*i] == '\"')
		handle_quote_token(input, i, j, tokens);
	else
		handle_word_token(input, i, j, tokens);
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
