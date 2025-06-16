/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 09:51:34 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/16 14:05:19 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_identifier(char *str)
{
	if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	return (1);
}

void	remove_env_var(t_env **env, char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	ft_unset(t_cmd *cmd, t_env **env)
{
	int	i;
	int	return_value;

	i = 1;
	return_value = 0;
	while (cmd->args[i])
	{
		if (is_valid_identifier(cmd->args[i]))
			remove_env_var(env, cmd->args[i]);
		else
        {
            write(2, "unset: `", 8);
            write(2, cmd->args[i], ft_strlen(cmd->args[i]));
            write(2, "': not a valid identifier\n", 26);
			return_value = 1;
        }
		i++;
	}
	return (return_value);
}
