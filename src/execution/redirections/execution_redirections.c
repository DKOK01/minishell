/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_redirections.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 10:30:00 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/30 17:36:00 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

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

static int	handle_empty_command_redirections(t_cmd *cmd)
{
	int	saved_stdin;
	int	saved_stdout;
	int	ret;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (cmd->heredoc && cmd->heredoc_fd != -1)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	else if (cmd->infile)
	{
		ret = handle_infile(cmd);
		if (ret != 0)
			return (restore_redirections(saved_stdin, saved_stdout), ret);
	}
	if (cmd->outfile || cmd->append)
	{
		ret = handle_outfile(cmd);
		if (ret != 0)
			return (restore_redirections(saved_stdin, saved_stdout), ret);
	}
	return (restore_redirections(saved_stdin, saved_stdout), 0);
}

static int	process_single_cmd_heredoc(t_cmd *cmd, t_env **env)
{
	int	heredoc_result;

	if (cmd->heredoc)
	{
		heredoc_result = process_all_heredocs(cmd, *env);
		if (heredoc_result != 0)
		{
			if (heredoc_result == 130)
				return (130);
			return (1);
		}
	}
	return (0);
}

int	process_single_command(t_cmd *cmd, t_env **env)
{
	int	result;

	result = process_single_cmd_heredoc(cmd, env);
	if (result != 0)
		return (result);
	if (check_builtins(cmd))
		return (handle_builtin_cmd(cmd, env));
	else if (cmd->args && cmd->args[0] && cmd->args[0][0] != '\0')
		execute_command(cmd, *env);
	else
		result = handle_empty_command_redirections(cmd);
	return (g_exit_status);
}
