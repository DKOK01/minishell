/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:25:54 by aysadeq           #+#    #+#             */
/*   Updated: 2025/06/30 11:10:54 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_redirection(t_cmd *cmd, t_token **tokens, int *i)
{
	char	*file;
	int		file_quoted;

	file = NULL;
	if (tokens[*i + 1])
		file = tokens[*i + 1]->value;
	if (validate_redirection_file(file) == -1)
		return (-1);
	file_quoted = check_file_quoted(tokens[*i + 1]);
	set_redirection(cmd, tokens[*i]->value, file, file_quoted);
	*i += 1;
	return (0);
}

static int	handle_pipe_token(t_cmd **current, t_token **tokens, int i)
{
	if (check_pipe_syntax(tokens, i, current) == -1)
		return (-1);
	(*current)->next = new_cmd_node();
	if (!(*current)->next)
		return (-1);
	*current = (*current)->next;
	return (0);
}

static int	process_token(t_cmd **current, t_token **tokens, int *i)
{
	char	**new_args;

	if (!ft_strcmp(tokens[*i]->value, "|"))
	{
		if (handle_pipe_token(current, tokens, *i) == -1)
			return (-1);
	}
	else if (is_redirection_token(tokens[*i]->value))
	{
		if (handle_redirection(*current, tokens, i) == -1)
			return (-1);
	}
	else
	{
		new_args = add_arg((*current)->args, tokens[*i]->value);
		if (!new_args)
			return (-1);
		(*current)->args = new_args;
	}
	return (0);
}

static int	process_all_tokens(t_token **tokens, t_cmd **current, t_cmd *head)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (process_token(current, tokens, &i) == -1)
			return (free_cmd_list(head), -1);
		i++;
	}
	return (0);
}

t_cmd	*parse_tokens(t_token **tokens)
{
	t_cmd	*head;
	t_cmd	*current;

	head = new_cmd_node();
	if (!head)
		return (NULL);
	current = head;
	if (check_initial_pipe_error(tokens) == -1)
	{
		free_cmd_list(head);
		return (NULL);
	}
	if (process_all_tokens(tokens, &current, head) == -1)
		return (NULL);
	return (head);
}
