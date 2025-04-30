/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:06:28 by aysadeq           #+#    #+#             */
/*   Updated: 2025/04/30 09:12:01 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int main(void)
{
	char *line;
	char **tokens;
	int i;

	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			printf("exit\n");
			break;
		}
		if (*line)
		{
			add_history(line);
		}
		tokens = tokenize_input(line);
		i = 0;
		while (tokens && tokens[i])
		{
			printf("Token[%d]: %s\n", i, tokens[i]);
			free(tokens[i]);
			i++;
		}
		free(tokens);
	}
}
