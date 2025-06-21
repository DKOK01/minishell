/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:00:00 by aysadeq           #+#    #+#             */
/*   Updated: 2025/06/21 17:59:13 by aysadeq          ###   ########.fr       */
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
