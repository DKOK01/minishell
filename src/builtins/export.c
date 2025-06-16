/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 09:57:00 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/16 14:01:34 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_if_valid(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
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

t_env	*create_env_node(char *str, char *eq)
{
	t_env	*new;
	char	*key;
	char	*value;
	int		len;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	if (eq)
	{
		len = eq - str;
		key = ft_substr(str, 0, len);
		value = ft_strdup(eq + 1);
	}
	else
	{
		key = ft_strdup(str);
		value = NULL;
	}
	if (!key || (eq && !value))
		return (free(key), free(value), free(new), NULL);
	new->key = key;
	new->value = value;
	new->next = NULL;
	return (new);
}

void	add_env_var(t_env **env, char *str)
{
	t_env	*node;
	t_env	*last;
	char	*eq;
	char	*key;

	eq = ft_strchr(str, '=');
	if (eq)
	{
		key = ft_substr(str, 0, eq - str);
		if (!key)
			return ;
		if (find_and_update(*env, key, eq + 1))
		{
			free(key);			
			return ;
    }
		free(key);
	}
	node = create_env_node(str, eq);
	if (!node)
		return ;
	if (!*env)
		*env = node;
	else
	{
		last = *env;
		while (last->next)
			last = last->next;
		last->next = node;
	}
}

int	ft_export(t_cmd *cmd, t_env **env)
{
	int		i;
	int		return_value;
	t_env	*tmp;

	i = 0;
	return_value = 0;
	if (!cmd->args[1])
	{
		tmp = *env;
		return (print_sorted_export(tmp), 0);
	}
	while (cmd->args[++i])
	{
		if (!check_if_valid(cmd->args[i]))
		{
			printf("export: '%s': not a valid identifier\n", cmd->args[i]);
			return_value = 1;
		}
		else
			add_env_var(env, cmd->args[i]);
	}
	return (return_value);
}
