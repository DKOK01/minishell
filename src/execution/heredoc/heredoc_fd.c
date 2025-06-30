/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_fd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:30:00 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/30 17:20:02 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	read_heredoc_input(char *delimiter, int pipe_fd, t_env *env,
		int should_expand)
{
	char	*line;

	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			g_exit_status = 130;
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc_line(pipe_fd, line, env, should_expand);
		free(line);
	}
	setup_parent_signals();
}

static int	handle_parent_process(int pipe_fd[2], pid_t pid)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if ((WIFEXITED(status) && WEXITSTATUS(status) == 130)
		|| WIFSIGNALED(status))
	{
		close(pipe_fd[0]);
		g_exit_status = 130;
		return (-1);
	}
	return (pipe_fd[0]);
}

int	create_heredoc_fd(char *delimiter, t_env *env, int should_expand)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		close(pipe_fd[0]);
		read_heredoc_input(delimiter, pipe_fd[1], env, should_expand);
		close(pipe_fd[1]);
		exit(0);
	}
	else if (pid > 0)
		return (handle_parent_process(pipe_fd, pid));
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	return (-1);
}
