/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 00:46:16 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/21 10:13:07 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern int g_exit_status;

static void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static void	setup_child_pipe(t_cmd *cmd, int prev_fd, int *pipe_fd, t_env **env)
{
	int	exit_status;

	setup_child_signals();
	if (cmd->heredoc)
		handle_heredoc(cmd, *env);
	else if (cmd->infile)
		handle_infile(cmd);
	else if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cmd->next)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	if (cmd->outfile || cmd->append)
		handle_outfile(cmd);
	if (check_builtins(cmd))
	{
		exit_status = run_builtin(cmd, env);
		exit(exit_status);
	}
	else if (cmd->args && cmd->args[0])
	{
		execute_command_direct(cmd, *env);
		exit(127);
	}
	else
		exit(0);
}

static void	handle_pipe_creation(t_cmd *cmd, int *pipe_fd)
{
	if (cmd->next && pipe(pipe_fd) < 0)
	{
		perror("pipe");
		exit(1);
	}
}

static pid_t	create_child_process(t_cmd *cmd, int prev_fd, int *pipe_fd, t_env **env)
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

static void	wait_for_children(pid_t last_pid, int *status)
{
	pid_t	wpid;
	int		tmp_status;

	// Wait for all children, but track the last one specifically
	while ((wpid = waitpid(-1, &tmp_status, 0)) > 0)
	{
		if (wpid == last_pid)
			*status = tmp_status;
	}
	// Update global exit status based on last command in pipeline
	if (WIFEXITED(*status))
		g_exit_status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
		g_exit_status = 128 + WTERMSIG(*status);
}

int	handle_pipeline(t_cmd *cmd, t_env **env)
{
	int		pipe_fd[2];
	int		prev_fd;
	pid_t	pid;
	pid_t	last_pid;
	int		status;

	prev_fd = -1;
	status = 0;
	last_pid = -1;
	while (cmd)
	{
		handle_pipe_creation(cmd, pipe_fd);
		pid = create_child_process(cmd, prev_fd, pipe_fd, env);
		last_pid = pid;
		if (prev_fd != -1)
			close(prev_fd);
		if (cmd->next)
		{
			close(pipe_fd[1]);
			prev_fd = pipe_fd[0];
		}
		cmd = cmd->next;
	}
	wait_for_children(last_pid, &status);
	return (g_exit_status);
}
