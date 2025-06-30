/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:04:48 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/30 12:47:33 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_env(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

t_env	**env_list_to_array(t_env *env)
{
	int		i;
	int		count;
	t_env	**array;

	i = 0;
	count = count_env(env);
	array = malloc(sizeof(t_env *) * (count + 1));
	if (!array)
		return (NULL);
	while (env)
	{
		array[i++] = env;
		env = env->next;
	}
	array[i] = NULL;
	return (array);
}

void	sort_env_array(t_env **array)
{
	int		i;
	t_env	*tmp;
	int		j;

	i = 0;
	while (array[i])
	{
		j = 0;
		while (array[j + 1])
		{
			if (ft_strcmp(array[j]->key, array[j + 1]->key) > 0)
			{
				tmp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_sorted_export(t_env *env)
{
	t_env	**array;
	int		i;

	array = env_list_to_array(env);
	if (!array)
		return ;
	sort_env_array(array);
	i = 0;
	while (array[i])
	{
		if (array[i]->value)
			printf("declare -x %s=\"%s\"\n", array[i]->key, array[i]->value);
		else
			printf("declare -x %s\n", array[i]->key);
		i++;
	}
	free(array);
}

t_env	*find_and_update(t_env *env, char *key, char *value)
{
	while (env)
	{
		if (!ft_strncmp(env->key, key, ft_strlen(key))
			&& env->key[ft_strlen(key)] == '\0')
		{
			free(env->value);
			env->value = ft_strdup(value);
			return (env);
		}
		env = env->next;
	}
	return (NULL);
}
