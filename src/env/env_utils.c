/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 08:50:59 by aysadeq           #+#    #+#             */
/*   Updated: 2025/05/06 10:03:53 by aysadeq          ###   ########.fr       */
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

