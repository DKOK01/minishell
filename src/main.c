/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:06:28 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/13 09:23:07 by aysadeq          ###   ########.fr       */
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

void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
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
	char	**tokens;
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
		i = 0;
		while (tokens[i])
		{
			tokens[i] = expand_variable(tokens[i], env);
			i++;
		}
		cmd = parse_tokens(tokens);
		if (cmd)
		{
			if (run_builtin(cmd, env) == -1)
				printf("Command not found\n");
		}
		// print_cmd_list(cmd);
		free_tokens(tokens);
		free_cmd_list(cmd);
		free(line);
	}
	free(line);
	free_env_list(env);
	return (0);
}
