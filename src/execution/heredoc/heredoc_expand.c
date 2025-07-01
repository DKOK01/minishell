/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 10:30:00 by ael-mans          #+#    #+#             */
/*   Updated: 2025/07/01 09:17:31 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*process_heredoc_char(const char *line, int *i, t_env *env,
		char *result)
{
	int	in_single;
	int	in_double;

	in_single = 0;
	in_double = 0;
	if (line[*i] == '\'' && !in_double)
		in_single = !in_single;
	else if (line[*i] == '"' && !in_single)
		in_double = !in_double;
	else if (line[*i] == '$' && (ft_isalpha(line[*i + 1])
			|| line[*i + 1] == '_'))
		return (handle_variable_expansion(line, i, env, result));
	else if (line[*i] == '$' && line[*i + 1] == '?')
		return (handle_exit_status_expansion(result, i));
	return (append_char_to_result(result, line[*i]));
}

static void	extract_heredoc_var_name(const char *line, int *i, char *var_name)
{
	int	var_start;

	(*i)++;
	var_start = 0;
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
	{
		var_name[var_start++] = line[*i];
		(*i)++;
	}
	var_name[var_start] = '\0';
	(*i)--;
}

char	*handle_variable_expansion(const char *line, int *i, t_env *env,
		char *result)
{
	char	var_name[256];
	char	*var_value;
	char	*temp;

	extract_heredoc_var_name(line, i, var_name);
	var_value = get_env_value(env, var_name);
	if (var_value)
	{
		temp = ft_strjoin(result, var_value);
		free(result);
		return (temp);
	}
	return (result);
}

char	*handle_exit_status_expansion(char *result, int *i)
{
	char	*exit_str;
	char	*temp;

	exit_str = ft_itoa(g_exit_status);
	temp = ft_strjoin_free(result, exit_str);
	(*i)++;
	return (temp);
}

char	*append_char_to_result(char *result, char c)
{
	char	*temp;

	temp = ft_strjoin_char(result, c);
	free(result);
	return (temp);
}
