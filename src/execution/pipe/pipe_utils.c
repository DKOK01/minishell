/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 10:30:00 by ael-mans          #+#    #+#             */
/*   Updated: 2025/07/04 09:00:27 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	setup_child_input(t_cmd *cmd, int prev_fd)
{
	int	ret;

	if (cmd->heredoc)
	{
		if (cmd->heredoc_fd != -1)
		{
			dup2(cmd->heredoc_fd, STDIN_FILENO);
			close(cmd->heredoc_fd);
		}
	}
	else if (cmd->infile)
	{
		ret = handle_infile(cmd);
		if (ret != 0)
			return (ret);
	}
	else if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	return (0);
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
	exit_status = setup_child_input(cmd, prev_fd);
	if (exit_status != 0)
		exit(exit_status);
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

	*status = 0;
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
