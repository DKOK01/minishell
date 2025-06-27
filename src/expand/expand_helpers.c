/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:00:00 by aysadeq           #+#    #+#             */
/*   Updated: 2025/06/27 19:45:35 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	append_env_value(char **result, char *var, t_env *env)
{
	char	*env_value;
	char	*temp;

	env_value = get_env_value(env, var);
	if (env_value)
	{
		temp = ft_strjoin_free(*result, ft_strdup(env_value));
		*result = temp;
	}
}

void	append_exit_status(char **result)
{
	char	*status_str;
	char	*temp;

	status_str = ft_itoa(g_exit_status);
	temp = ft_strjoin_free(*result, status_str);
	*result = temp;
}

void	expand_handle_variable_expansion(const char *token, t_expand_ctx *ctx)
{
	char	var[256];

	(*(ctx->i))++;
	extract_var_name(token, ctx->i, var);
	append_env_value(ctx->result, var, ctx->env);
}

void	expand_handle_exit_status(t_expand_ctx *ctx)
{
	(*(ctx->i)) += 2;
	append_exit_status(ctx->result);
}

void	expand_append_char_to_result(t_expand_ctx *ctx, const char *token)
{
	char	*temp;

	temp = ft_strjoin_char(*(ctx->result), token[*(ctx->i)]);
	free(*(ctx->result));
	*(ctx->result) = temp;
}
