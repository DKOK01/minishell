/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:06:28 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/01 15:08:56 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_cmd_list(t_cmd *cmd)
{
	t_cmd	*current;

	current = cmd;
	while (current)
	{
		printf("Command:\n");
		if (current->args)
		{
			for (int i = 0; current->args[i]; i++)
				printf("  Arg %d: %s\n", i, current->args[i]);
		}
		if (current->infile)
			printf("  Infile: %s\n", current->infile);
		if (current->outfile)
			printf("  Outfile: %s\n", current->outfile);
		if (current->append)
			printf("  Append: %d\n", current->append);
		if (current->heredoc)
			printf("  Heredoc: %d\n", current->heredoc);
		current = current->next;
	}
}

int	main(void)
{
	t_cmd	*cmd;
	char	*line;
	char	**tokens;

	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		tokens = tokenize_input(line);
		cmd = parse_tokens(tokens);
		print_cmd_list(cmd);
		free(tokens);
	}
}
