/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_envp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:29:40 by ael-mans          #+#    #+#             */
/*   Updated: 2025/05/23 11:58:15 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
                return (NULL);
            ft_strlcpy(envp[i], env->key, ft_strlen(env->key) + 1);
			ft_strlcat(envp[i], "=", len);
			ft_strlcat(envp[i], env->value, len);
            i++;
        }
        env = env->next;
    }
	envp[i] = NULL;
    return (envp);
}
