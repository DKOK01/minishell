/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:06:28 by aysadeq           #+#    #+#             */
/*   Updated: 2025/07/04 11:01:16 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_exit_status;

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

static void	expand_tokens(t_token **tokens, t_env *env)
{
	int		i;
	char	*expanded;

	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->quoted != 1 && tokens[i]->seg_count > 0)
		{
			expanded = expand_token_segments(tokens[i], env);
			free(tokens[i]->value);
			tokens[i]->value = expanded;
		}
		i++;
	}
}

void print_cmd_list(t_cmd *cmd)
{
	int	i;

	if (!cmd)
	{
		printf("No commands found.\n");
		return ;
	}
	printf("\n=== COMMAND LIST ===\n");
	i = 0;
	while (cmd)
	{
		printf("Command [%d]:\n", i);
		printf("	Arguments:			");
		if (cmd->args && cmd->args[0])
		{
			int j = 0;
			while (cmd->args[j])
			{
				printf("[%s] ", cmd->args[j]);
				j++;
			}
			printf("\n");
		}
		else
			printf("[None]\n");
		printf("	Input file:			[%s]\n", cmd->infile ? cmd->infile : "None");
		printf("	Output file:			[%s]\n", cmd->outfile ? cmd->outfile : "None");
		printf("	Append mode:			[%d]\n", cmd->append);
		printf("	Heredoc mode:			[%d]\n", cmd->heredoc);
		printf("	Heredoc quoted:		[%d]\n", cmd->heredoc_quoted);
		printf("	Heredoc file descriptor: [%d]\n", cmd->heredoc_fd);
		cmd = cmd->next;
		i++;
	}
	printf("=== END COMMAND LIST ===\n\n");
}

static void	process_input_line(char *line, t_env **env)
{
	t_cmd	*cmd;
	t_token	**tokens;

	tokens = tokenize_input(line);
	if (!tokens)
		return ;
	print_token_list(tokens);
	expand_tokens(tokens, *env);
	cmd = parse_tokens(tokens);
	print_cmd_list(cmd);
	if (cmd)
		execution(cmd, env);
	free_tokens(tokens);
	free_cmd_list(cmd);
}

static void	main_loop(t_env *env)
{
	char	*line;

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
		else
		{
			free(line);
			continue ;
		}
		process_input_line(line, &env);
		free(line);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env;

	(void)ac;
	(void)av;
	env = create_env(envp);
	setup_parent_signals();
	main_loop(env);
	free_env_list(env);
	return (0);
}
