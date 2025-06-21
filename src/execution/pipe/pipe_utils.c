/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 10:30:00 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/21 19:54:23 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static void	setup_child_input(t_cmd *cmd, int prev_fd, t_env **env)
{
	if (cmd->heredoc)
		handle_heredoc(cmd, *env);
	else if (cmd->infile)
		handle_infile(cmd);
	else if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
}

static void	setup_child_output(t_cmd *cmd, int *pipe_fd)
{
	if (cmd->next)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	if (cmd->outfile || cmd->append)
		handle_outfile(cmd);
}

void	setup_child_pipe(t_cmd *cmd, int prev_fd, int *pipe_fd, t_env **env)
{
	int	exit_status;

	setup_child_signals();
	setup_child_input(cmd, prev_fd, env);
	setup_child_output(cmd, pipe_fd);
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

void	wait_for_children(pid_t last_pid, int *status)
{
	pid_t	wpid;
	int		tmp_status;

	wpid = waitpid(-1, &tmp_status, 0);
	while (wpid > 0)
	{
		if (wpid == last_pid)
			*status = tmp_status;
		wpid = waitpid(-1, &tmp_status, 0);
	}
	if (WIFEXITED(*status))
		g_exit_status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
		g_exit_status = 128 + WTERMSIG(*status);
}
