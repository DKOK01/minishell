/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_segments_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:30:00 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/27 17:24:47 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

char	*extract_unquoted_part(char *input, int *i)
{
	char	*result;
	int		start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i])
		&& input[*i] != '|' && input[*i] != '>' && input[*i] != '<'
		&& input[*i] != '"' && input[*i] != '\'')
		(*i)++;
	result = ft_substr(input, start, *i - start);
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

	segment_value = extract_unquoted_part(input, i);
	if (ft_strlen(segment_value) > 0)
	{
		add_segment_to_token(token, segment_value, 0);
		*full_value = ft_strjoin_free(*full_value, segment_value);
	}
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
