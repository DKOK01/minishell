/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:04:29 by aysadeq           #+#    #+#             */
/*   Updated: 2025/06/21 11:42:04 by ael-mans         ###   ########.fr       */
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
# include <sys/stat.h>
# include <signal.h>

extern int g_exit_status;

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
	int				heredoc_quoted;
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
void		append_exit_status(char **result);
void		extract_var_name(const char *token, int *i, char *var);

//------ parser functions--------//
t_cmd		*parse_tokens(t_token **tokens);
void		free_cmd_list(t_cmd *cmd);
t_cmd		*new_cmd_node(void);
char		**add_arg(char **args, char *token);
int			validate_redirection_file(char *file);
void		set_redirection(t_cmd *cmd, char *operator, char *file, int quoted);
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
int			ft_pwd(t_env *env);
t_env		*find_and_update(t_env *env, char *key, char *value);
int			ft_export(t_cmd *cmd, t_env **env);
int			ft_unset(t_cmd *cmd, t_env **env);
int			ft_env(t_cmd *cmd, t_env *env);
void		print_sorted_export(t_env *env);
int			execution(t_cmd *cmd, t_env **env);
int			check_builtins(t_cmd *cmd);
int			count_env(t_env *env);
int			handle_pipeline(t_cmd *cmd, t_env **env);
int			execute_command(t_cmd *cmd, t_env *env);
void		execute_command_direct(t_cmd *cmd, t_env *env);
int			handle_heredoc(t_cmd *cmd, t_env *env);
void		ft_free_split(char **split);
void		set_env_value(t_env **env, const char *key, const char *value);
int			process_single_command(t_cmd *cmd, t_env **env);
char		*find_path(char *cmd, t_env *env);
void		handle_command_not_found(char *cmd_name, char **envp, int env_count);
void		setup_child_pipe(t_cmd *cmd, int prev_fd, int *pipe_fd, t_env **env);
void		wait_for_children(pid_t last_pid, int *status);
char		*handle_variable_expansion(const char *line, int *i, t_env *env,
				char *result);
char		*handle_exit_status_expansion(char *result, int *i);
char		*append_char_to_result(char *result, char c);
void		write_heredoc_line(int pipe_fd, char *line, t_env *env,
				int should_expand);
int			setup_heredoc_pipe(int *pipe_fd);
char		*expand_heredoc_line(const char *line, t_env *env);

//------- redirection functions--------//
int			check_redirection(t_cmd *cmd, t_env *env);
int			handle_infile(t_cmd *cmd);
int			handle_outfile(t_cmd *cmd);

//------- execution utils--------//
char		*ft_strjoin_three(const char *s1, const char *s2, const char *s3);
void		ft_free_split(char **split);

//------- signal handling--------//
void		setup_parent_signals(void);
void		sigint_handler(int signum);


#endif