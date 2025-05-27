/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:03:13 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/27 16:23:45 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	extract_var_name(const char *token, int *i, char *var)
{
	int	j;

	j = 0;
	while (token[*i] && (ft_isalnum(token[*i]) || token[*i] == '_'))
		var[j++] = token[(*i)++];
	var[j] = '\0';
}

static void	append_env_value(char **result, char *var, t_env *env)
{
	char	*var_value;
	char	*temp;

	var_value = get_env_value(env, var);
	if (!var_value)
		var_value = "";
	temp = ft_strjoin(*result, var_value);
	free(*result);
	*result = temp;
}

void	handle_expansion(const char *token, t_expand_ctx *ctx)
{
	char	var[256];
	char	*temp;

	if (ctx->quoted == 2)
	{
		if (token[*(ctx->i)] == '\"')
			ctx->in_double = !(ctx->in_double);
		else if (token[*(ctx->i)] == '$'
			&& (ft_isalpha(token[*(ctx->i) + 1]) || token[*(ctx->i) + 1] == '_'))
		{
			(*(ctx->i))++;
			extract_var_name(token, ctx->i, var);
			append_env_value(ctx->result, var, ctx->env);
			return ;
		}
		else
		{
			temp = ft_strjoin_char(*(ctx->result), token[*(ctx->i)]);
			free(*(ctx->result));
			*(ctx->result) = temp;
		}
	}
	else if (ctx->quoted == 1)
	{
		temp = ft_strjoin_char(*(ctx->result), token[*(ctx->i)]);
		free(*(ctx->result));
		*(ctx->result) = temp;
	}
	else
	{
		if (token[*(ctx->i)] == '\'')
			ctx->in_single = !(ctx->in_single);
		else if (token[*(ctx->i)] == '\"')
			ctx->in_double = !(ctx->in_double);
		else if (token[*(ctx->i)] == '$' && !(ctx->in_single)
			&& (ft_isalpha(token[*(ctx->i) + 1]) || token[*(ctx->i) + 1] == '_'))
		{
			(*(ctx->i))++;
			extract_var_name(token, ctx->i, var);
			append_env_value(ctx->result, var, ctx->env);
			return ;
		}
		else
		{
			temp = ft_strjoin_char(*(ctx->result), token[*(ctx->i)]);
			free(*(ctx->result));
			*(ctx->result) = temp;
		}
	}
	(*(ctx->i))++;
}

char	*expand_variable(char *token, t_env *env, int quoted)
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

	if (!token || !token->segments)
		return (expand_variable(token->value, env, token->quoted));
	result = ft_strdup("");
	i = 0;
	while (i < token->seg_count)
	{
		expanded_segment = expand_variable(token->segments[i]->value, env, token->segments[i]->quote_type);
		temp = ft_strjoin_free(result, expanded_segment);
		result = temp;
		free(expanded_segment);
		i++;
	}
	return (result);
}
