/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 10:30:00 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/21 11:42:04 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern int	g_exit_status;

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
