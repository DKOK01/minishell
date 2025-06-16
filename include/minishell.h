/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:04:29 by aysadeq           #+#    #+#             */
/*   Updated: 2025/06/16 13:59:52 by aysadeq          ###   ########.fr       */
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

typedef struct s_segment
{
	char	*value;
	int		quote_type;
}	t_segment;

typedef struct s_token
{
	char		*value;
	t_segment	**segments;
	int			seg_count;
	int			quoted;
}	t_token;

typedef struct s_cmd
{
	char			**args;
	char			*infile;
	char			*outfile;
	int				append;
	int				heredoc;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_expand_ctx
{
	int		*i;
	char	**result;
	t_env	*env;
	int		in_single;
	int		in_double;
	int		quoted;
}	t_expand_ctx;

//------ lexer functions--------//
t_token		**tokenize_input(char *input);
void		handle_word_with_segments(char *input, int *i, int *j, t_token **tokens);
t_token		*make_token_with_segments(void);
t_segment	*create_segment(char *value, int quote_type);
void		free_segments(t_segment **segments);
void		free_tokens(t_token **tokens);

//------ expansion functions--------//
char		*expand_variable(char *token, t_env *env, int quoted);
char		*expand_token_segments(t_token *token, t_env *env);
char		*ft_strjoin_char(char *str, char c);
char		*ft_strjoin_free(char *s1, char *s2);
void		append_env_value(char **result, char *var, t_env *env);
void		extract_var_name(const char *token, int *i, char *var);

//------ parser functions--------//
t_cmd		*parse_tokens(t_token **tokens);
void		free_cmd_list(t_cmd *cmd);
t_cmd		*new_cmd_node(void);
char		**add_arg(char **args, char *token);
int			validate_redirection_file(char *file);
void		set_redirection(t_cmd *cmd, char *operator, char *file);
int			is_redirection_token(char *token);

//------- environment functions--------//
void		free_env_list(t_env *env);
t_env		*create_env(char **envp);
char		*get_env_value(t_env *env, const char *key);
char		**env_to_envp(t_env *env);
void		free_envp(char **envp, int n);

//------- execution functions--------//
int			run_builtin(t_cmd *cmd, t_env **env);
int			ft_echo(t_cmd *cmd);
int			ft_cd(t_cmd *cmd, t_env **env);
int			ft_exit(t_cmd *cmd);
int			ft_pwd(void);
int			ft_export(t_cmd *cmd, t_env **env);
int			ft_unset(t_cmd *cmd, t_env **env);
int			ft_env(t_env *env);
void		print_sorted_export(t_env *env);
int			execution(t_cmd *cmd, t_env **env);
int			check_builtins(t_cmd *cmd);
int			count_env(t_env *env);
int			handle_pipeline(t_cmd *cmd, t_env **env);
void		execute_command(t_cmd *cmd, t_env *env);
int			handle_heredoc(t_cmd *cmd);
void		ft_free_split(char **split);
void		set_env_value(t_env **env, const char *key, const char *value);

//------- redirection functions--------//
int			check_redirection(t_cmd *cmd);
int			handle_infile(t_cmd *cmd);
int			handle_outfile(t_cmd *cmd);

//------- execution utils--------//
char		*ft_strjoin_three(const char *s1, const char *s2, const char *s3);
void		ft_free_split(char **split);

#endif