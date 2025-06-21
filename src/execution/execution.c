/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 09:18:40 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/21 08:33:01 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
extern int g_exit_status;

char	*find_path(char *cmd, t_env *env)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	if (cmd[0] == '/')
		return (ft_strdup(cmd));
	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin_three(paths[i], "/", cmd);
		if (access(full_path, X_OK) == 0)
			return (ft_free_split(paths), full_path);
		free(full_path);
		i++;
	}
	ft_free_split(paths);
	return (NULL);
}

void	handle_command_not_found(char *cmd_name, char **envp, int env_count)
{
	struct stat	file_stat;

	if (stat(cmd_name, &file_stat) == 0)
	{
		write(2, cmd_name, ft_strlen(cmd_name));
		write(2, ": is a directory\n", 17);
	}
	else
	{
		write(2, cmd_name, ft_strlen(cmd_name));
		write(2, ": command not found\n", 20);
	}
	free_envp(envp, env_count);
}

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

void	execute_command_direct(t_cmd *cmd, t_env *env)
{
	char	*path;
	char	**envp;
	int		env_count;

	envp = env_to_envp(env);
	env_count = count_env(env);
	path = find_path(cmd->args[0], env);
	if (!path)
	{
		handle_command_not_found(cmd->args[0], envp, env_count);
		exit(127);
	}
	execve(path, cmd->args, envp);
	perror(cmd->args[0]);
	exit(127);
}

static void	handle_builtin_redirections(t_cmd *cmd, int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (cmd->infile && cmd->heredoc == 0)
		handle_infile(cmd);
	if (cmd->heredoc)
		handle_heredoc(cmd);
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
	if (cmd->heredoc)
		handle_heredoc(cmd);
	else if (cmd->infile)
		handle_infile(cmd);
	if (cmd->outfile || cmd->append)
		handle_outfile(cmd);
}

static int	process_single_command(t_cmd *cmd, t_env **env)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = -1;
	saved_stdout = -1;
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

int	execution(t_cmd *cmd, t_env **env)
{
	int	exit_status;

	exit_status = 0;
	if (cmd && cmd->next)
	{
		exit_status = handle_pipeline(cmd, env);
		return (0);
	}
	while (cmd)
	{
		process_single_command(cmd, env);
		cmd = cmd->next;
	}
	return (g_exit_status);
}
