/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_io.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 10:30:00 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/22 16:28:59 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	write_heredoc_line(int pipe_fd, char *line, t_env *env,
		int should_expand)
{
	char	*expanded_line;

	if (should_expand)
	{
		expanded_line = expand_heredoc_line(line, env);
		write(pipe_fd, expanded_line, ft_strlen(expanded_line));
		free(expanded_line);
	}
	else
		write(pipe_fd, line, ft_strlen(line));
	write(pipe_fd, "\n", 1);
}

int	setup_heredoc_pipe(int *pipe_fd)
{
	if (pipe(pipe_fd) < 0)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

int	process_all_heredocs(t_cmd *cmd, t_env *env)
{
	t_cmd	*current;
	char	*delimiter;
	int		should_expand;

	current = cmd;
	while (current)
	{
		if (current->heredoc)
		{
			delimiter = ft_strdup(current->infile);
			if (!delimiter)
				return (1);
			should_expand = !current->heredoc_quoted;
			current->heredoc_fd = create_heredoc_fd(delimiter, env,
				should_expand);
			free(delimiter);
			if (current->heredoc_fd == -1)
				return (1);
		}
		current = current->next;
	}
	return (0);
}
