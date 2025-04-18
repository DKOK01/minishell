/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:15:17 by aysadeq           #+#    #+#             */
/*   Updated: 2025/04/12 18:47:54 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**tokenize_input(char *input)
{
	char	**tokens;
	int		i;
	int		j;
	int		start;
	char	quote;

	tokens = malloc(sizeof(char *) * 100);
	if (!tokens)
		return (NULL);
	i = 0;
	j = 0;
	while (input[i])
	{
		skip_spaces(input, &i);
		if (input[i] == '\0')
			break ;
		if (input[i] == '\"' || input[i] == '\'')
		{
			quote = input[i++];
			start = i;
			while (input[i] && input[i] != quote)
				i++;
			tokens[j++] = ft_substr(input, start, i - start);
			if (input[i] == quote)
				i++;
		}
		else if ((input[i] == '>' || input[i] == '<') && input[i] == input[i + 1])
		{
			tokens[j++] = ft_substr(input, i, 2);
			i += 2;
		}
		else if (



