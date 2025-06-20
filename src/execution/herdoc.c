/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 01:03:20 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/20 10:52:17 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_heredoc(t_cmd *cmd)
{
	char	*line;
	char	*delimiter;
	int		pipe_fd[2];

	delimiter = ft_strdup(cmd->infile);
    if (!delimiter)
	    return (1);
	if (pipe(pipe_fd) < 0)
	{
		perror("pipe");
		free(delimiter);
		exit(1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
    free(delimiter);
	return (0);
}
