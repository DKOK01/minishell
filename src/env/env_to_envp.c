/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_envp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:29:40 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/16 12:33:00 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	join_key_value(char *dest, const char *key, const char *value)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (key[i])
	{
		dest[i] = key[i];
		i++;
	}
	dest[i++] = '=';
	while (value[j])
	{
		dest[i + j] = value[j];
		j++;
	}
	dest[i + j] = '\0';
}

void	free_envp(char **envp, int n)
{
	int	idx;

	idx = 0;
	while (idx < n)
	{
		free(envp[idx]);
		idx++;
	}
	free(envp);
}

char	**env_to_envp(t_env *env)
{
	int		count;
	char	**envp;
	int		i;
	int		len;

	i = 0;
	count = count_env(env);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	while (env)
	{
		if (env->value)
		{
			len = ft_strlen(env->key) + ft_strlen(env->value) + 2;
			envp[i] = malloc(len);
			if (!envp[i])
				return (free_envp(envp, i), NULL);
			join_key_value(envp[i], env->key, env->value);
			i++;
		}
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}
