/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 08:50:59 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/12 09:57:04 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*create_env(char **envp)
{
	t_env	*new;
	t_env	*head;
	char	*equal;
	int		i;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		equal = ft_strchr(envp[i], '=');
		if (equal)
		{
			new = malloc(sizeof(t_env));
			if (!new)
				return (NULL);
			new->key = ft_substr(envp[i], 0, equal - envp[i]);
			new->value = ft_strdup(equal + 1);
			new->next = head;
			head = new;
		}
		i++;
	}
	return (head);
}

char	*get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return ("");
}
