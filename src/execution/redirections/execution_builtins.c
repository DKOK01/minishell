/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_builtins.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:35:00 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/30 17:35:00 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	handle_builtin_redirections(t_cmd *cmd, int *saved_stdin,
		int *saved_stdout)
{
	int	ret;

	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (cmd->infile && cmd->heredoc == 0)
	{
		ret = handle_infile(cmd);
		if (ret != 0)
			return (ret);
	}
	if (cmd->heredoc && cmd->heredoc_fd != -1)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	if (cmd->outfile || cmd->append)
	{
		ret = handle_outfile(cmd);
		if (ret != 0)
			return (ret);
	}
	return (0);
}

static void	restore_redirections(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

int	handle_builtin_cmd(t_cmd *cmd, t_env **env)
{
	int	saved_stdin;
	int	saved_stdout;
	int	result;

	saved_stdin = -1;
	saved_stdout = -1;
	if (cmd->infile || cmd->append || cmd->heredoc || cmd->outfile)
	{
		result = handle_builtin_redirections(cmd, &saved_stdin,
				&saved_stdout);
		if (result != 0)
			return (g_exit_status);
	}
	g_exit_status = run_builtin(cmd, env);
	restore_redirections(saved_stdin, saved_stdout);
	return (g_exit_status);
}
