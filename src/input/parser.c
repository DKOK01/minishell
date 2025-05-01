/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:46:59 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/01 09:33:26 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd	*new_cmd_node(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->next = NULL;
	return (cmd);
}

char	**create_arg_list(char *token)
{
	char	**new_args;

	new_args = malloc(sizeof(char *) * 2);
	if (!new_args)
		return (NULL);
	new_args[0] = ft_strdup(token);
	new_args[1] = NULL;
	return (new_args);
}

static char	**add_arg(char **args, char *token)
{
	int		i;
	char	**new_args;

	if (!args)
		return (create_arg_list(token));
	i = 0;
	while (args[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return (NULL);
	while (--i >= 0)
		new_args[i] = args[i];
	new_args[++i] = ft_strdup(token);
	new_args[i + 1] = NULL;
	free(args);
	return (new_args);
}

void	handle_redirection(t_cmd *cmd, char **tokens, int *i)
{
	if (!ft_strcmp(tokens[*i], "<"))
		cmd->infile = ft_strdup(tokens[++(*i)]);
	else if (!ft_strcmp(tokens[*i], "<<"))
	{
		cmd->infile = ft_strdup(tokens[++(*i)]);
		cmd->heredoc = 1;
	}
	else if (!ft_strcmp(tokens[*i], ">"))
		cmd->outfile = ft_strdup(tokens[++(*i)]);
	else if (!ft_strcmp(tokens[*i], ">>"))
	{
		cmd->outfile = ft_strdup(tokens[++(*i)]);
		cmd->append = 1;
	}
}

t_cmd	*parse_tokens(char **tokens)
{
	t_cmd	*head;
	t_cmd	*current;
	int		i;

	head = new_cmd_node();
	current = head;
	i = 0;
	while (tokens[i])
	{
		if (!ft_strcmp(tokens[i], "|"))
		{
			current->next = new_cmd_node();
			current = current->next;
		}
		else if (!ft_strcmp(tokens[i], "<") || !ft_strcmp(tokens[i], ">")
			|| !ft_strcmp(tokens[i], "<<") || !ft_strcmp(tokens[i], ">>"))
			handle_redirection(current, tokens, &i);
		else
			current->args = add_arg(current->args, tokens[i]);
		i++;
	}
	return (head);
}
