/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:15:17 by aysadeq           #+#    #+#             */
/*   Updated: 2025/04/30 11:02:39 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	skip_spaces(char *s, int *i)
{
	while (s[*i] && ft_isspace(s[*i]))
		(*i)++;
}

int	is_special(char c)
{
	return (c == '>' || c == '<' || c == '|');
}

char	*get_quoted_string(char *s, int *i)
{
	char	quote = s[*i];
	int		start;

	(*i)++;
	start = *i;
	while (s[*i] && s[*i] != quote)
		(*i)++;
	if (s[*i] == quote)
	{
		char *result = ft_substr(s, start, *i - start);
		(*i)++;
		return result;
	}
	return ft_substr(s, start, *i - start);
}


char	**init_token_array(void)
{
	char	**tokens = malloc(sizeof(char *) * 1024);
	if (tokens)
		tokens[0] = NULL;
	return (tokens);
}

char	**tokenize_input(char *s)
{
	int		i = 0, j = 0, start;
	char	**tokens = init_token_array();

	if (!tokens || !s)
		return (NULL);
	while (s[i])
	{
		skip_spaces(s, &i);
		if (!s[i])
			break;
		if (s[i] == '\'' || s[i] == '"')
			tokens[j++] = get_quoted_string(s, &i);
		else if ((s[i] == '>' || s[i] == '<') && s[i] == s[i + 1])
		{
			tokens[j++] = ft_substr(s, i, 2);
			i += 2;
		}
		else if (is_special(s[i]))
		{
			tokens[j++] = ft_substr(s, i, 1);
			i++;
		}
		else
		{
			start = i;
			while (s[i] && !ft_isspace(s[i]) && !is_special(s[i]))
				i++;
			tokens[j++] = ft_substr(s, start, i - start);
		}
	}
	tokens[j] = NULL;
	return (tokens);
}
