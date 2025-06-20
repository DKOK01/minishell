/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 09:18:40 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/20 16:30:43 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*find_path(char *cmd, t_env *env)
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

int	execute_command(t_cmd *cmd, t_env *env)
{
	char	*path;
	pid_t	pid;
	int 	status;
	char	**envp;
	int		env_count;
	struct stat	file_stat;

	envp = env_to_envp(env);
	env_count = count_env(env);
	path = find_path(cmd->args[0], env);
	status = 0;
	if (!path)
	{
		if (stat(cmd->args[0], &file_stat) == 0)
		{
			write(2, cmd->args[0], ft_strlen(cmd->args[0]));
			write(2, ": is a directory\n", 17);
		}
		else
		{
			write(2, cmd->args[0], ft_strlen(cmd->args[0]));
			write(2, ": command not found\n", 20);
		}
		free_envp(envp, env_count);
		return (1);
	}
	pid = fork();
	if (pid == 0)
	{
		if (cmd->infile || cmd->append || cmd->heredoc || cmd->outfile)
			check_redirection(cmd);
		execve(path, cmd->args, envp);
		perror(cmd->args[0]);
		exit(0);
	}
	else if (pid > 0)
		waitpid(pid, &status, 0);
	free(path);
	free_envp(envp, env_count);
	return (status);
}

int	execution(t_cmd *cmd, t_env **env)
{
    int		status;
    int		saved_stdout;
    int		saved_stdin;
	int		exit_status;

    status = 0;
	exit_status = 0;
    if (cmd && cmd->next)
    {
        exit_status = handle_pipeline(cmd, env);
        return (0);
    }
    while (cmd)
    {
        if (check_builtins(cmd))
        {
            saved_stdin = -1;
            saved_stdout = -1;
            if (cmd->infile || cmd->append || cmd->heredoc || cmd->outfile)
            {
                saved_stdin = dup(STDIN_FILENO);
                saved_stdout = dup(STDOUT_FILENO);
                if (cmd->infile && cmd->heredoc == 0)
                    handle_infile(cmd);
                if (cmd->heredoc)
					handle_heredoc(cmd);
				if (cmd->outfile || cmd->append)
					handle_outfile(cmd);
            }
            exit_status = run_builtin(cmd, env);
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
        else if (cmd->args && cmd->args[0] && cmd->args[0][0] != '\0')
            exit_status = execute_command(cmd, *env);
        else
        {
            if (cmd->heredoc)
                handle_heredoc(cmd);
            else if (cmd->infile)
                handle_infile(cmd);
            if (cmd->outfile || cmd->append)
                handle_outfile(cmd);
        }
        cmd = cmd->next;
    }
    return (exit_status);
}
