/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 13:54:01 by aysadeq           #+#    #+#             */
/*   Updated: 2025/06/16 14:00:08 by aysadeq          ###   ########.fr       */
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

char	**add_arg(char **args, char *token)
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

int	validate_redirection_file(char *file)
{
	if (!file)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	if (!ft_strcmp(file, "|"))
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		return (-1);
	}
	return (0);
}

void	set_redirection(t_cmd *cmd, char *operator, char *file)
{
	if (!ft_strcmp(operator, "<"))
		cmd->infile = ft_strdup(file);
	else if (!ft_strcmp(operator, "<<"))
	{
		cmd->infile = ft_strdup(file);
		cmd->heredoc = 1;
	}
	else if (!ft_strcmp(operator, ">"))
		cmd->outfile = ft_strdup(file);
	else if (!ft_strcmp(operator, ">>"))
	{
		cmd->outfile = ft_strdup(file);
		cmd->append = 1;
	}
}

int	is_redirection_token(char *token)
{
	return (!ft_strcmp(token, "<") || !ft_strcmp(token, ">")
		|| !ft_strcmp(token, "<<") || !ft_strcmp(token, ">>"));
}
