/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:03:13 by aysadeq           #+#    #+#             */
/*   Updated: 2025/06/29 18:12:56 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_exit_status(t_expand_ctx *ctx)
{
	char	*status_str;
	char	*temp;

	(*(ctx->i)) += 2;
	status_str = ft_itoa(g_exit_status);
	temp = ft_strjoin_free(*(ctx->result), status_str);
	*(ctx->result) = temp;
	free(status_str);
}

static void	variable_expansion(const char *segment, t_expand_ctx *ctx)
{
	char	*var;
	int		start;
	int		end;
	char	*env_value;
	char	*temp;

	start = (*(ctx->i)) + 1;
	end = start;
	while (segment[end] && (ft_isalnum(segment[end])
			|| segment[end] == '_'))
		end++;
	var = ft_substr(segment, start, end - start);
	env_value = get_env_value(ctx->env, var);
	temp = ft_strjoin_free(*(ctx->result), env_value);
	*(ctx->result) = temp;
	free(var);
	*(ctx->i) = end;
}

static void	handle_expansion(const char *segment, t_expand_ctx *ctx)
{
	char	*temp;

	if (segment[*(ctx->i)] == '$' && (ft_isalpha(segment[*(ctx->i) + 1])
			|| segment[*(ctx->i) + 1] == '_'))
	{
		variable_expansion(segment, ctx);
		return ;
	}
	else if (segment[*(ctx->i)] == '$' && segment[*(ctx->i) + 1] == '?')
	{
		handle_exit_status(ctx);
		return ;
	}
	else
	{
		temp = ft_strjoin_char(*(ctx->result), segment[*(ctx->i)]);
		free(*(ctx->result));
		*(ctx->result) = temp;
		(*(ctx->i))++;
	}
}

static char	*expand_variable(char *segment, t_env *env, int quoted)
{
	char			*result;
	int				i;
	t_expand_ctx	ctx;

	if (quoted == 1)
		return (ft_strdup(segment));
	result = ft_strdup("");
	i = 0;
	ctx.i = &i;
	ctx.result = &result;
	ctx.env = env;
	ctx.quoted = quoted;
	while (segment[i])
		handle_expansion(segment, &ctx);
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
