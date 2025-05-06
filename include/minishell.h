/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:04:29 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/06 10:31:20 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>

typedef struct s_cmd
{
	char	**args;
	char	*infile;
	char	*outfile;
	int		append;
	int		heredoc;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	char	*key;
	char	*value;
	struct s_env	*next;
}	t_env;

char	**tokenize_input(char *input);
t_cmd	*parse_tokens(char **tokens);

int		run_builtin(t_cmd *cmd, t_env *env);
int		ft_echo(t_cmd *cmd);
int		ft_cd(t_cmd *cmd);
int		ft_exit(t_cmd *cmd);

#endif