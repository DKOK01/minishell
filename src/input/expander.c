/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:03:13 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/15 15:05:53 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_strjoin_char(char *str, char c)
{
	int		len;
	char	*new_str;
	int		i;

	if (!str)
	{
		new_str = malloc(2);
		if (!new_str)
			return (NULL);
		new_str[0] = c;
		new_str[1] = '\0';
		return (new_str);
	}
	len = ft_strlen(str);
	new_str = malloc(len + 2);
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i++] = c;
	new_str[i] = '\0';
	return (new_str);
}

char	*expand_variable(char *token, t_env *env)
{
	char	*result = ft_strdup("");
	char	*temp;
	char	var[256];
	char	*var_value;
	int		i = 0, j;
	int		in_single = 0, in_double = 0;

	while (token[i])
	{
		if (token[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (token[i] == '\"' && !in_single)
			in_double = !in_double;
		else if (token[i] == '$' && !in_single &&
			(ft_isalnum(token[i + 1]) || token[i + 1] == '_'))
		{
			i++;
			j = 0;
			while (token[i] && (ft_isalnum(token[i]) || token[i] == '_'))
				var[j++] = token[i++];
			var[j] = '\0';
			var_value = get_env_value(env, var);
			temp = ft_strjoin(result, var_value);
			free(result);
			result = temp;
			continue;
		}
		else
		{
			temp = ft_strjoin_char(result, token[i]);
			free(result);
			result = temp;
		}
		i++;
	}
	return (result);
}

