/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:00:00 by aysadeq           #+#    #+#             */
/*   Updated: 2025/06/21 17:59:13 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_variable_expansion(const char *token, t_expand_ctx *ctx)
{
	char	var[256];

	(*(ctx->i))++;
	extract_var_name(token, ctx->i, var);
	append_env_value(ctx->result, var, ctx->env);
}

void	handle_exit_status(t_expand_ctx *ctx)
{
	(*(ctx->i)) += 2;
	append_exit_status(ctx->result);
}

void	append_char_to_result(t_expand_ctx *ctx, const char *token)
{
	char	*temp;

	temp = ft_strjoin_char(*(ctx->result), token[*(ctx->i)]);
	free(*(ctx->result));
	*(ctx->result) = temp;
}
