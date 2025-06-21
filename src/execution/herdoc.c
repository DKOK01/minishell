/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 01:03:20 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/21 10:14:31 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*expand_heredoc_line(const char *line, t_env *env)
{
	char	*result;
	char	*temp;
	int		i;
	int		in_single_quotes;
	int		in_double_quotes;
	char	var_name[256];
	int		var_start;

	result = ft_strdup("");
	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	
	while (line[i])
	{
		if (line[i] == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			temp = ft_strjoin_char(result, line[i]);
			free(result);
			result = temp;
		}
		else if (line[i] == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			temp = ft_strjoin_char(result, line[i]);
			free(result);
			result = temp;
		}
		else if (line[i] == '$' && (ft_isalpha(line[i + 1]) || line[i + 1] == '_'))
		{
			// Extract variable name - expand variables even inside single quotes in heredoc
			i++;
			var_start = 0;
			while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
			{
				var_name[var_start++] = line[i];
				i++;
			}
			var_name[var_start] = '\0';
			i--; // Back up one since we'll increment at end of loop
			
			// Get variable value and append
			char *var_value = get_env_value(env, var_name);
			if (var_value)
			{
				temp = ft_strjoin_free(result, ft_strdup(var_value));
				result = temp;
			}
		}
		else if (line[i] == '$' && line[i + 1] == '?')
		{
			// Handle exit status - expand even inside single quotes in heredoc
			char *exit_str = ft_itoa(g_exit_status);
			temp = ft_strjoin_free(result, exit_str);
			result = temp;
			i++; // Skip the '?'
		}
		else if (line[i] == '$' && in_single_quotes)
		{
			// In single quotes, preserve $ literally
			temp = ft_strjoin_char(result, line[i]);
			free(result);
			result = temp;
		}
		else
		{
			temp = ft_strjoin_char(result, line[i]);
			free(result);
			result = temp;
		}
		i++;
	}
	return (result);
}

int	handle_heredoc(t_cmd *cmd, t_env *env)
{
	char	*line;
	char	*delimiter;
	char	*expanded_line;
	int		pipe_fd[2];
	int		should_expand;

	delimiter = ft_strdup(cmd->infile);
    if (!delimiter)
	    return (1);
	
	// Check if delimiter was quoted during parsing
	should_expand = !cmd->heredoc_quoted;
	
	if (pipe(pipe_fd) < 0)
	{
		perror("pipe");
		free(delimiter);
		exit(1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		
		// Expand variables if delimiter was not quoted
		if (should_expand)
		{
			expanded_line = expand_heredoc_line(line, env);
			write(pipe_fd[1], expanded_line, ft_strlen(expanded_line));
			free(expanded_line);
		}
		else
		{
			write(pipe_fd[1], line, ft_strlen(line));
		}
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
    free(delimiter);
	return (0);
}
