/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:15:17 by aysadeq           #+#    #+#             */
/*   Updated: 2025/04/12 18:23:10 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

char	**tokenize_input(char *input)
{
	char	**tokens;
	int		i;
	int		j;
	int		start;

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
		if 
