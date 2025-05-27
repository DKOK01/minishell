/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:25:54 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/27 17:28:54 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_cmd	*new_cmd_node(void)
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

static char	**add_arg(char **args, char *token)
{
	int		i;
	int		j;
	char	**new_args;

	if (!args)
	{
		new_args = malloc(sizeof(char *) * 2);
		new_args[0] = ft_strdup(token);
		new_args[1] = NULL;
		return (new_args);
	}
	i = 0;
	while (args[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	j = 0;
	while (j < i)
	{
		new_args[j] = args[j];
		j++;
	}
	new_args[i] = ft_strdup(token);
	new_args[i + 1] = NULL;
	free(args);
	return (new_args);
}

static void	handle_redirection(t_cmd *cmd, t_token **tokens, int *i)
{
	char	*file;

	file = NULL;
	if (tokens[*i + 1])
		file = tokens[*i + 1]->value;
	if (!ft_strcmp(tokens[*i]->value, "<"))
		cmd->infile = ft_strdup(file);
	else if (!ft_strcmp(tokens[*i]->value, "<<"))
	{
		cmd->infile = ft_strdup(file);
		cmd->heredoc = 1;
	}
	else if (!ft_strcmp(tokens[*i]->value, ">"))
		cmd->outfile = ft_strdup(file);
	else if (!ft_strcmp(tokens[*i]->value, ">>"))
	{
		cmd->outfile = ft_strdup(file);
		cmd->append = 1;
	}
	*i += 1;
}

t_cmd	*parse_tokens(t_token **tokens)
{
	t_cmd	*head;
	t_cmd	*current;
	int		i;

	head = new_cmd_node();
	current = head;
	i = 0;
	while (tokens[i])
	{
		if (!ft_strcmp(tokens[i]->value, "|"))
		{
			current->next = new_cmd_node();
			current = current->next;
		}
		else if (!ft_strcmp(tokens[i]->value, "<") || 
			!ft_strcmp(tokens[i]->value, ">") || 
			!ft_strcmp(tokens[i]->value, "<<") || 
			!ft_strcmp(tokens[i]->value, ">>"))
			handle_redirection(current, tokens, &i);
		else
			current->args = add_arg(current->args, tokens[i]->value);
		i++;
	}
	return (head);
}
