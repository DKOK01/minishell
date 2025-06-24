/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_redirections.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 10:30:00 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/21 10:30:00 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	handle_builtin_redirections(t_cmd *cmd, int *saved_stdin,
		int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (cmd->infile && cmd->heredoc == 0)
		handle_infile(cmd);
	if (cmd->heredoc && cmd->heredoc_fd != -1)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	if (cmd->outfile || cmd->append)
		handle_outfile(cmd);
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

static void	handle_empty_command_redirections(t_cmd *cmd)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (cmd->heredoc && cmd->heredoc_fd != -1)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	else if (cmd->infile)
		handle_infile(cmd);
	if (cmd->outfile || cmd->append)
		handle_outfile(cmd);
	restore_redirections(saved_stdin, saved_stdout);
}

int	process_single_command(t_cmd *cmd, t_env **env)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = -1;
	saved_stdout = -1;
	if (cmd->heredoc)
	{
		if (process_all_heredocs(cmd, *env) != 0)
			return (1);
	}
	if (check_builtins(cmd))
	{
		if (cmd->infile || cmd->append || cmd->heredoc || cmd->outfile)
			handle_builtin_redirections(cmd, &saved_stdin, &saved_stdout);
		g_exit_status = run_builtin(cmd, env);
		restore_redirections(saved_stdin, saved_stdout);
	}
	else if (cmd->args && cmd->args[0] && cmd->args[0][0] != '\0')
		execute_command(cmd, *env);
	else
		handle_empty_command_redirections(cmd);
	return (g_exit_status);
}
