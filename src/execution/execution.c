/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 09:18:40 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/22 16:34:47 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_child_process(t_cmd *cmd, char *path, char **envp)
{
	if (cmd->infile || cmd->append || cmd->heredoc || cmd->outfile)
		check_redirection(cmd);
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
	if (cmd && cmd->next)
	{
		handle_pipeline(cmd, env);
		return (0);
	}
	while (cmd)
	{
		process_single_command(cmd, env);
		cmd = cmd->next;
	}
	return (g_exit_status);
}

int	execution(t_cmd *cmd, t_env **env)
{
	return (execute_pipeline_or_single(cmd, env));
}
