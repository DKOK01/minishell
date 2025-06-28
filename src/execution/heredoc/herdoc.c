/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azedine <azedine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 01:03:20 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/28 17:33:57 by azedine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*process_heredoc_char(const char *line, int *i, t_env *env,
		char *result)
{
	int	in_single;
	int	in_double;

	in_single = 0;
	in_double = 0;
	if (line[*i] == '\'' && !in_double)
		in_single = !in_single;
	else if (line[*i] == '"' && !in_single)
		in_double = !in_double;
	else if (line[*i] == '$' && (ft_isalpha(line[*i + 1])
			|| line[*i + 1] == '_'))
		return (handle_variable_expansion(line, i, env, result));
	else if (line[*i] == '$' && line[*i + 1] == '?')
		return (handle_exit_status_expansion(result, i));
	return (append_char_to_result(result, line[*i]));
}

char	*expand_heredoc_line(const char *line, t_env *env)
{
	char	*result;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (line[i])
	{
		result = process_heredoc_char(line, &i, env, result);
		i++;
	}
	return (result);
}

static void	read_heredoc_input(char *delimiter, int pipe_fd, t_env *env,
		int should_expand)
{
	char	*line;

	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			g_exit_status = 130;
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc_line(pipe_fd, line, env, should_expand);
		free(line);
	}
	setup_parent_signals();
}

int	create_heredoc_fd(char *delimiter, t_env *env, int should_expand)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipe_fd) == -1)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		close(pipe_fd[0]);
		read_heredoc_input(delimiter, pipe_fd[1], env, should_expand);
		close(pipe_fd[1]);
		exit(0);
	}
	else if (pid > 0)
	{
		close(pipe_fd[1]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		{
			close(pipe_fd[0]);
			g_exit_status = 130;
			return (-1);
		}
		return (pipe_fd[0]);
	}
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	return (-1);
}

int	handle_heredoc(t_cmd *cmd, t_env *env)
{
	char	*delimiter;
	int		heredoc_fd;
	int		should_expand;

	delimiter = ft_strdup(cmd->infile);
	if (!delimiter)
		return (1);
	should_expand = !cmd->heredoc_quoted;
	heredoc_fd = create_heredoc_fd(delimiter, env, should_expand);
	free(delimiter);
	if (heredoc_fd == -1)
		return (1);
	dup2(heredoc_fd, STDIN_FILENO);
	close(heredoc_fd);
	return (0);
}
