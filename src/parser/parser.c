/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:25:54 by aysadeq           #+#    #+#             */
/*   Updated: 2025/06/27 18:21:59 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_redirection(t_cmd *cmd, t_token **tokens, int *i)
{
	char	*file;
	int		file_quoted;
	int		j;

	file = NULL;
	if (tokens[*i + 1])
		file = tokens[*i + 1]->value;
	if (validate_redirection_file(file) == -1)
		return (-1);
	file_quoted = 0;
	if (tokens[*i + 1] && tokens[*i + 1]->segments)
	{
		j = 0;
		while (j < tokens[*i + 1]->seg_count)
		{
			if (tokens[*i + 1]->segments[j]->quote_type > 0)
			{
				file_quoted = 1;
				break ;
			}
			j++;
		}
	}
	set_redirection(cmd, tokens[*i]->value, file, file_quoted);
	*i += 1;
	return (0);
}

static int	handle_pipe_token(t_cmd **current, t_token **tokens, int i)
{
	if (!tokens[i + 1] || !ft_strcmp(tokens[i + 1]->value, "|"))
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		return (-1);
	}
	if (!(*current)->args)
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		return (-1);
	}
	(*current)->next = new_cmd_node();
	*current = (*current)->next;
	return (0);
}

static int	process_token(t_cmd **current, t_token **tokens, int *i)
{
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
		(*current)->args = add_arg((*current)->args, tokens[*i]->value);
	return (0);
}

t_cmd	*parse_tokens(t_token **tokens)
{
	t_cmd	*head;
	t_cmd	*current;
	int		i;

	head = new_cmd_node();
	current = head;
	i = 0;
	if (tokens[0] && !ft_strcmp(tokens[0]->value, "|"))
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		free(head);
		return (NULL);
	}
	while (tokens[i])
	{
		if (process_token(&current, tokens, &i) == -1)
		{
			free_cmd_list(head);
			return (NULL);
		}
		i++;
	}
	return (head);
}
