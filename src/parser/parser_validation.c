/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:55:00 by aysadeq           #+#    #+#             */
/*   Updated: 2025/06/30 11:09:12 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	validate_redirection_file(char *file)
{
	if (!file)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		g_exit_status = 2;
		return (-1);
	}
	if (!ft_strcmp(file, "|"))
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		g_exit_status = 2;
		return (-1);
	}
	return (0);
}

int	is_redirection_token(char *token)
{
	return (!ft_strcmp(token, "<") || !ft_strcmp(token, ">")
		|| !ft_strcmp(token, "<<") || !ft_strcmp(token, ">>"));
}

int	check_pipe_syntax(t_token **tokens, int i, t_cmd **current)
{
	if (!tokens[i + 1])
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		g_exit_status = 2;
		return (-1);
	}
	if (!ft_strcmp(tokens[i + 1]->value, "|"))
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		g_exit_status = 2;
		return (-1);
	}
	if (!(*current)->args)
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		g_exit_status = 2;
		return (-1);
	}
	return (0);
}

int	check_initial_pipe_error(t_token **tokens)
{
	if (tokens[0] && !ft_strcmp(tokens[0]->value, "|"))
	{
		printf("minishell: syntax error near unexpected token `|'\n");
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
