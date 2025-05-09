/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:03:13 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/06 18:55:48 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char *expand_variable(char *token, t_env *env)
{
	char	*expanded;
	char	*value;
	char	*var_name;
	int		i;

	i = 0;
	while (token[i] && token[i] != '$')
		i++;
	if (token[i] == '$')
	{
		var_name = ft_strdup(token + i + 1);
		value = get_env_value(env, var_name);
		free(var_name);
		if (!value)
			return (ft_strdup(token));
		expanded = ft_strjoin(ft_substr(token, 0, i), value);
		free(token);
		return (expanded);
	}
	return (ft_strdup(token));
}

