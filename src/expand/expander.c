/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:03:13 by aysadeq           #+#    #+#             */
/*   Updated: 2025/06/28 15:03:02 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_double_quoted(const char *token, t_expand_ctx *ctx)
{
	if (token[*(ctx->i)] == '\"')
		ctx->in_double = !(ctx->in_double);
	else if (token[*(ctx->i)] == '$'
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
		expand_append_char_to_result(ctx, token);
	(*(ctx->i))++;
}

static void	handle_unquoted(const char *token, t_expand_ctx *ctx)
{
	if (token[*(ctx->i)] == '\'')
		ctx->in_single = !(ctx->in_single);
	else if (token[*(ctx->i)] == '\"')
		ctx->in_double = !(ctx->in_double);
	else if (token[*(ctx->i)] == '$' && !(ctx->in_single)
		&& (ft_isalpha(token[*(ctx->i) + 1])
			|| token[*(ctx->i) + 1] == '_'))
	{
		expand_handle_variable_expansion(token, ctx);
		return ;
	}
	else if (token[*(ctx->i)] == '$' && !(ctx->in_single)
		&& token[*(ctx->i) + 1] == '?')
	{
		expand_handle_exit_status(ctx);
		return ;
	}
	else
		expand_append_char_to_result(ctx, token);
	(*(ctx->i))++;
}

static void	handle_expansion(const char *token, t_expand_ctx *ctx)
{
	char	*temp;

	if (ctx->quoted == 2)
		handle_double_quoted(token, ctx);
	else if (ctx->quoted == 1)
	{
		temp = ft_strjoin_char(*(ctx->result), token[*(ctx->i)]);
		free(*(ctx->result));
		*(ctx->result) = temp;
		(*(ctx->i))++;
	}
	else
		handle_unquoted(token, ctx);
}

static char	*expand_variable(char *token, t_env *env, int quoted)
{
	char			*result;
	int				i;
	t_expand_ctx	ctx;

	result = ft_strdup("");
	i = 0;
	ctx.i = &i;
	ctx.result = &result;
	ctx.env = env;
	ctx.in_single = 0;
	ctx.in_double = 0;
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
