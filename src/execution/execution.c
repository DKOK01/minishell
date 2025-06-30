/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 09:18:40 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/30 14:35:57 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_child_process(t_cmd *cmd, char *path, char **envp)
{
	int	ret;

	if (cmd->infile || cmd->append || cmd->heredoc || cmd->outfile)
	{
		ret = check_redirection(cmd);
		if (ret != 0)
			exit(ret);
	}
	execve(path, cmd->args, envp);
	perror(cmd->args[0]);
	exit(127);
}

static void	handle_parent_process(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
}

int	execute_command(t_cmd *cmd, t_env *env)
{
	char	*path;
	pid_t	pid;
	char	**envp;
	int		env_count;

	envp = env_to_envp(env);
	env_count = count_env(env);
	path = find_path(cmd->args[0], env);
	if (!path)
	{
		handle_command_not_found(cmd->args[0], envp, env_count);
		g_exit_status = 127;
		return (127);
	}
	pid = fork();
	if (pid == 0)
		handle_child_process(cmd, path, envp);
	else if (pid > 0)
		handle_parent_process(pid);
	free(path);
	free_envp(envp, env_count);
	return (g_exit_status);
}

static int	execute_pipeline_or_single(t_cmd *cmd, t_env **env)
{
	int	result;

	if (cmd->next)
	{
		result = handle_pipeline(cmd, env);
		if (result == 130)
			return (130);
		return (0);
	}
	result = process_single_command(cmd, env);
	if (result == 130)
		return (130);
	return (g_exit_status);
}

int	execution(t_cmd *cmd, t_env **env)
{
	int	result;

	setup_exec_signals();
	result = execute_pipeline_or_single(cmd, env);
	setup_parent_signals();
	return (result);
}
