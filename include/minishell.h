/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:04:29 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/24 15:01:41 by aysadeq          ###   ########.fr       */
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
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_token
{
	char	*value;
	int		quoted;
}	t_token;

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

typedef struct s_expand_ctx
{
	int		*i;
	char	**result;
	t_env	*env;
	int		in_single;
	int		in_double;
}	t_expand_ctx;

//------ lexer functions--------//
t_token	**tokenize_input(char *input);
t_token	*make_token(char *value, int quoted);
char	*extract_word(char *input, int *i);
char	*extract_quoted(char *s, int *i, int *qtype);
void	handle_redir_token(char *input, int *i, int *j, t_token **tokens);
void	handle_single_token(char *input, int *i, int *j, t_token **tokens);
void	handle_quote_token(char *input, int *i, int *j, t_token **tokens);
void	handle_word_token(char *input, int *i, int *j, t_token **tokens);
void	skip_spaces(char *input, int *i);
void	append_quoted_part(char **result, char *input, int *i);
void	free_tokens(t_token **tokens);
void	test_lexer_cases(void);
void	print_token_list(t_token **tokens);

//------ parser functions--------//
t_cmd	*parse_tokens(t_token **tokens);

//------ expansion functions--------//
char 	*expand_variable(char *token, t_env *env);
char	*ft_strjoin_char(char *str, char c);

//------- environment functions--------//
void	free_env_list(t_env *env);
t_env	*create_env(char **envp);
char	*get_env_value(t_env *env, const char *key);
char	**env_to_envp(t_env *env);

//------- execution functions--------//
int		run_builtin(t_cmd *cmd, t_env *env);
int		ft_echo(t_cmd *cmd);
int		ft_cd(t_cmd *cmd, t_env *env);
int		ft_exit(t_cmd *cmd);
int		ft_pwd(void);
int		ft_export(t_cmd *cmd, t_env **env);
int 	ft_unset(t_cmd *cmd, t_env **env);
int 	ft_env(t_env *env);
void	print_sorted_export(t_env *env);
int		execution(t_cmd *cmd, t_env *env);
int		check_builtins(t_cmd *cmd);
int		count_env(t_env *env);

#endif