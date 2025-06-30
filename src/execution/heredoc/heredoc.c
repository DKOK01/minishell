/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 01:03:20 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/30 17:20:11 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*process_heredoc_char(const char *line, int *i, t_env *env,
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

char	*expand_heredoc_line(const char *line, t_env *env)
{
	char	*result;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (line[i])
	{
		result = process_heredoc_char(line, &i, env, result);
		if (!result)
			return (NULL);
		i++;
	}
	return (result);
}

int	handle_heredoc(t_cmd *cmd, t_env *env)
{
	char	*delimiter;
	int		heredoc_fd;
	int		should_expand;

	delimiter = ft_strdup(cmd->infile);
	if (!delimiter)
		return (1);
	should_expand = !cmd->heredoc_quoted;
	heredoc_fd = create_heredoc_fd(delimiter, env, should_expand);
	free(delimiter);
	if (heredoc_fd == -1)
		return (1);
	dup2(heredoc_fd, STDIN_FILENO);
	close(heredoc_fd);
	return (0);
}
