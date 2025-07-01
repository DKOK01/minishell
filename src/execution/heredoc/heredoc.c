/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 01:03:20 by ael-mans          #+#    #+#             */
/*   Updated: 2025/07/01 09:18:31 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

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
