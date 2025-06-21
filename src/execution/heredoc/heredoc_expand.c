/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 10:30:00 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/21 14:38:49 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

extern int	g_exit_status;

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
		temp = ft_strjoin_free(result, ft_strdup(var_value));
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
