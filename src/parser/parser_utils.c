/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 13:54:01 by aysadeq           #+#    #+#             */
/*   Updated: 2025/07/04 14:31:43 by aysadeq          ###   ########.fr       */
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
	cmd->heredoc_quoted = 0;
	cmd->heredoc_fd = -1;
	cmd->next = NULL;
	return (cmd);
}

char	**add_arg(char **args, char *token)
{
	char	**new_args;
	int		count;
	int		j;

	count = 0;
	if (args)
		while (args[count])
			count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return (NULL);
	j = -1;
	while (++j < count)
		new_args[j] = args[j];
	new_args[count] = ft_strdup(token);
	if (!new_args[count])
		return (free(new_args), NULL);
	new_args[count + 1] = NULL;
	if (args)
		free(args);
	return (new_args);
}

void	set_redirection(t_cmd *cmd, char *operator, char *file, int quoted)
{
	if (!ft_strcmp(operator, "<"))
		cmd->infile = ft_strdup(file);
	else if (!ft_strcmp(operator, "<<"))
	{
		cmd->infile = ft_strdup(file);
		cmd->heredoc = 1;
		cmd->heredoc_quoted = quoted;
	}
	else if (!ft_strcmp(operator, ">"))
		cmd->outfile = ft_strdup(file);
	else if (!ft_strcmp(operator, ">>"))
	{
		cmd->outfile = ft_strdup(file);
		cmd->append = 1;
	}
}

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*temp;
	int		i;

	while (cmd)
	{
		temp = cmd;
		cmd = cmd->next;
		if (temp->args)
		{
			i = 0;
			while (temp->args[i])
			{
				free(temp->args[i]);
				i++;
			}
			free(temp->args);
		}
		if (temp->infile)
			free(temp->infile);
		if (temp->outfile)
			free(temp->outfile);
		free(temp);
	}
}

int	is_redirection_token(char *token)
{
	return (!ft_strcmp(token, "<") || !ft_strcmp(token, ">")
		|| !ft_strcmp(token, "<<") || !ft_strcmp(token, ">>"));
}
