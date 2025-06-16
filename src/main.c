/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:06:28 by aysadeq           #+#    #+#             */
/*   Updated: 2025/06/16 13:17:33 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_token_list(t_token **tokens)
{
	int	i;
	int	j;

	if (!tokens)
	{
		printf("No tokens found.\n");
		return ;
	}
	printf("\n=== TOKEN LIST ===\n");
	i = 0;
	while (tokens[i])
	{
		printf("Token [%d]:\n", i);
		printf("	Value:				[%s]\n", tokens[i]->value);
		printf("	Overall quoted:		%d\n", tokens[i]->quoted);
		printf("	Segment count:		%d\n", tokens[i]->seg_count);
		
		if (tokens[i]->segments && tokens[i]->seg_count > 0)
		{
			printf("	Segments:\n");
			j = 0;
			while (j < tokens[i]->seg_count)
			{
				printf("	[%d]- -> [%s] (quote_type: %d", j, 
					tokens[i]->segments[j]->value, 
					tokens[i]->segments[j]->quote_type);
				if (tokens[i]->segments[j]->quote_type == 0)
					printf(" - unquoted");
				else if (tokens[i]->segments[j]->quote_type == 1)
					printf(" - single quoted");
				else if (tokens[i]->segments[j]->quote_type == 2)
					printf(" - double quoted");
				printf(")\n");
				j++;
			}
		}
		else
		{
			printf("  No segments (legacy token)\n");
		}
		printf("\n");
		i++;
	}
	printf("=== END TOKEN LIST ===\n\n");
}

void	print_cmd_list(t_cmd *cmd)
{
	t_cmd	*current;

	current = cmd;
	while (current)
	{
		printf("\nCommand:\n");
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

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*temp;

	while (cmd)
	{
		temp = cmd;
		cmd = cmd->next;
		if (temp->args)
			free(temp->args);
		if (temp->infile)
			free(temp->infile);
		if (temp->outfile)
			free(temp->outfile);
		free(temp);
	}
}

void	free_env_list(t_env *env)
{
	t_env	*temp;

	while (env)
	{
		temp = env;
		env = env->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}



int	main(int ac, char **av, char **envp)
{
	t_cmd	*cmd;
	t_env	*env;
	char	*line;
	t_token	**tokens;
	int		i;

	(void)ac;
	(void)av;
	env = create_env(envp);
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
		if (!tokens)
		{
			free(line);
			continue;
		}
		// print_token_list(tokens);
		i = 0;
		while (tokens[i])
		{
			if (tokens[i]->quoted != 1)
			{
				char *expanded = expand_token_segments(tokens[i], env);
				free(tokens[i]->value);
				tokens[i]->value = expanded;
			}
			i++;
		}
		cmd = parse_tokens(tokens);
		if (cmd && cmd->args)
			execution(cmd, &env);
		else if (cmd && cmd->heredoc)
			handle_heredoc(cmd);
		// print_cmd_list(cmd);
		free_tokens(tokens);
		free_cmd_list(cmd);
		free(line);
	}
	free(line);
	free_env_list(env);
	return (0);
}
