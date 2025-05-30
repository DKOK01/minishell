/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_segments.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:30:00 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/30 10:01:05 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	add_segment_to_token(t_token *token, char *value, int quote_type)
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

static char	*extract_quoted(char *input, int *i, int *qtype)
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

static void	process_quoted_segment(char *input, int *i, t_token *token,
		char **full_value)
{
	char	*segment_value;
	int		quote_type;

	segment_value = extract_quoted(input, i, &quote_type);
	add_segment_to_token(token, segment_value, quote_type);
	*full_value = ft_strjoin_free(*full_value, segment_value);
	free(segment_value);
}

static void	process_unquoted_segment(char *input, int *i, t_token *token,
		char **full_value)
{
	char	*segment_value;
	char	*result;
	int		start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i])
		&& input[*i] != '|' && input[*i] != '>' && input[*i] != '<'
		&& input[*i] != '"' && input[*i] != '\'')
		(*i)++;
	segment_value = ft_substr(input, start, *i - start);
	add_segment_to_token(token, segment_value, 0);
	*full_value = ft_strjoin_free(*full_value, segment_value);
	free(segment_value);
}

void	handle_word_with_segments(char *input, int *i, int *j, t_token **tokens)
{
	t_token	*token;
	char	*full_value;

	token = make_token_with_segments();
	if (!token)
		return ;
	full_value = ft_strdup("");
	while (input[*i] && !ft_isspace(input[*i])
		&& input[*i] != '|' && input[*i] != '>' && input[*i] != '<')
	{
		if (input[*i] == '"' || input[*i] == '\'')
			process_quoted_segment(input, i, token, &full_value);
		else
			process_unquoted_segment(input, i, token, &full_value);
	}
	token->value = full_value;
	tokens[(*j)++] = token;
}
