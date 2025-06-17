/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 16:00:25 by aysadeq           #+#    #+#             */
/*   Updated: 2025/06/17 12:40:10 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_strjoin_char_null(char c)
{
	char	*new_str;

	new_str = malloc(2);
	if (!new_str)
		return (NULL);
	new_str[0] = c;
	new_str[1] = '\0';
	return (new_str);
}

char	*ft_strjoin_char(char *str, char c)
{
	int		len;
	char	*new_str;
	int		i;

	if (!str)
		return (ft_strjoin_char_null(c));
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

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	if (!s1 || !s2)
		return (NULL);
	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

void	extract_var_name(const char *token, int *i, char *var)
{
	int	j;

	j = 0;
	while (token[*i] && (ft_isalnum(token[*i]) || token[*i] == '_'))
		var[j++] = token[(*i)++];
	var[j] = '\0';
}

void	append_env_value(char **result, char *var, t_env *env)
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

void	append_exit_status(char **result)
{
	char	*status_str;
	char	*temp;

	status_str = ft_itoa(g_exit_status);
	temp = ft_strjoin(*result, status_str);
	free(*result);
	free(status_str);
	*result = temp;
}
