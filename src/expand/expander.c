/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:03:13 by aysadeq           #+#    #+#             */
/*   Updated: 2025/06/29 10:44:13 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	expand_handle_variable_expansion(const char *token, t_expand_ctx *ctx)
{
	char	var[256];
	int		j;
	char	*env_value;
	char	*temp;

	(*(ctx->i))++;
	j = 0;
	while (token[*(ctx->i)] && (ft_isalnum(token[*(ctx->i)]) || token[*(ctx->i)] == '_'))
		var[j++] = token[(*(ctx->i))++];
	var[j] = '\0';
	env_value = get_env_value(ctx->env, var);
	if (env_value)
	{
		temp = ft_strjoin_free(*(ctx->result), ft_strdup(env_value));
		*(ctx->result) = temp;
	}
}

static void	expand_handle_exit_status(t_expand_ctx *ctx)
{
	char	*status_str;
	char	*temp;
	
	(*(ctx->i)) += 2;
	status_str = ft_itoa(g_exit_status);
	temp = ft_strjoin_free(*(ctx->result), status_str);
	*(ctx->result) = temp;
}

static void	handle_expansion(const char *token, t_expand_ctx *ctx)
{
	char	*temp;

	if (token[*(ctx->i)] == '$'
		&& (ft_isalpha(token[*(ctx->i) + 1])
			|| token[*(ctx->i) + 1] == '_'))
	{
		expand_handle_variable_expansion(token, ctx);
		return ;
	}
	else if (token[*(ctx->i)] == '$' && token[*(ctx->i) + 1] == '?')
	{
		expand_handle_exit_status(ctx);
		return ;
	}
	else
	{
		temp = ft_strjoin_char(*(ctx->result), token[*(ctx->i)]);
		free(*(ctx->result));
		*(ctx->result) = temp;
		(*(ctx->i))++;
	}
}

static char	*expand_variable(char *token, t_env *env, int quoted)
{
	char			*result;
	int				i;
	t_expand_ctx	ctx;

	if (quoted == 1)
		return (ft_strdup(token));
	result = ft_strdup("");
	i = 0;
	ctx.i = &i;
	ctx.result = &result;
	ctx.env = env;
	ctx.quoted = quoted;
	while (token[i])
		handle_expansion(token, &ctx);
	return (result);
}

char	*expand_token_segments(t_token *token, t_env *env)
{
	char	*result;
	char	*expanded_segment;
	char	*temp;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (i < token->seg_count)
	{
		expanded_segment = expand_variable(token->segments[i]->value, env,
				token->segments[i]->quote_type);
		temp = ft_strjoin_free(result, expanded_segment);
		result = temp;
		free(expanded_segment);
		i++;
	}
	return (result);
}
