/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azedine <azedine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 00:46:16 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/28 16:29:14 by azedine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	handle_pipe_creation(t_cmd *cmd, int *pipe_fd)
{
	if (cmd->next && pipe(pipe_fd) < 0)
	{
		perror("pipe");
		exit(1);
	}
}

static pid_t	create_child_process(t_cmd *cmd, int prev_fd, int *pipe_fd,
		t_env **env)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
		setup_child_pipe(cmd, prev_fd, pipe_fd, env);
	return (pid);
}

static void	cleanup_pipe_fds(int prev_fd, t_cmd *cmd, int *pipe_fd)
{
	if (prev_fd != -1)
		close(prev_fd);
	if (cmd->next)
		close(pipe_fd[1]);
}

static int	process_pipeline_command(t_cmd **cmd, int *prev_fd, int *pipe_fd,
		t_env **env)
{
	pid_t	pid;

	handle_pipe_creation(*cmd, pipe_fd);
	pid = create_child_process(*cmd, *prev_fd, pipe_fd, env);
	cleanup_pipe_fds(*prev_fd, *cmd, pipe_fd);
	if ((*cmd)->next)
		*prev_fd = pipe_fd[0];
	*cmd = (*cmd)->next;
	return (pid);
}

int	handle_pipeline(t_cmd *cmd, t_env **env)
{
	int		pipe_fd[2];
	int		prev_fd;
	pid_t	last_pid;
	int		status;
	int		heredoc_result;

	heredoc_result = process_all_heredocs(cmd, *env);
	if (heredoc_result != 0)
	{
		if (heredoc_result == 130)
			return (130);
		return (1);
	}
	prev_fd = -1;
	status = 0;
	last_pid = -1;
	while (cmd)
		last_pid = process_pipeline_command(&cmd, &prev_fd, pipe_fd, env);
	wait_for_children(last_pid, &status);
	return (g_exit_status);
}
