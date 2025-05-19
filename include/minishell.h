/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azedine <azedine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:04:29 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/19 16:35:55 by azedine          ###   ########.fr       */
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

t_env	*create_env(char **envp);
char	*get_env_value(t_env *env, const char *key);
char 	*expand_variable(char *token, t_env *env);


int		run_builtin(t_cmd *cmd, t_env *env);
int		ft_echo(t_cmd *cmd);
int		ft_cd(t_cmd *cmd, t_env *env);
int		ft_exit(t_cmd *cmd);
int		ft_pwd(void);
int		ft_export(t_cmd *cmd, t_env **env);
int 	ft_unset(t_cmd *cmd, t_env **env);
int 	ft_env(t_env *env);
void	print_sorted_export(t_env *env);


#endif