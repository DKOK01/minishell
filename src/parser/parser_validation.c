/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:55:00 by aysadeq           #+#    #+#             */
/*   Updated: 2025/07/04 14:34:00 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	validate_redirection_file(char *file)
{
	if (!file)
	{
		ft_putstr_fd("minishell: syntax error near "
			"unexpected token `newline'\n", 2);
		g_exit_status = 2;
		return (-1);
	}
	if (!ft_strcmp(file, "|"))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		g_exit_status = 2;
		return (-1);
	}
	return (0);
}

int	check_pipe_syntax(t_token **tokens, int i, t_cmd **current)
{
	if (!tokens[i + 1])
	{
		ft_putstr_fd("minishell: syntax error near unexpected token "
			"`newline'\n", 2);
		g_exit_status = 2;
		return (-1);
	}
	if (!ft_strcmp(tokens[i + 1]->value, "|"))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		g_exit_status = 2;
		return (-1);
	}
	if (!(*current)->args)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		g_exit_status = 2;
		return (-1);
	}
	return (0);
}

int	check_initial_pipe_error(t_token **tokens)
{
	if (tokens[0] && !ft_strcmp(tokens[0]->value, "|"))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		g_exit_status = 2;
		return (-1);
	}
	return (0);
}

int	check_file_quoted(t_token *token)
{
	int	j;

	if (!token || !token->segments)
		return (0);
	j = 0;
	while (j < token->seg_count)
	{
		if (token->segments[j]->quote_type > 0)
			return (1);
		j++;
	}
	return (0);
}

int	check_invalid_redirection_sequence(t_token **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (is_redirection_token(tokens[i]->value))
		{
			if (tokens[i + 1] && is_redirection_token(tokens[i + 1]->value))
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `",
					2);
				ft_putstr_fd(tokens[i + 1]->value, 2);
				ft_putstr_fd("'\n", 2);
				g_exit_status = 2;
				return (-1);
			}
		}
		i++;
	}
	return (0);
}
