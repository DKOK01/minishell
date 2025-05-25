/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 09:58:09 by ael-mans          #+#    #+#             */
/*   Updated: 2025/05/25 03:36:22 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// int	ft_cd(t_cmd *cmd, t_env *env)
// {
// 	char	*path;

// 	path = cmd->args[1];
// 	if (!path)
// 	{
// 		path = get_env_value(env, "HOME");
// 		if (!path)
// 		{
// 			write(2, "cd: HOME not set\n", 17);
// 			return (1);
// 		}
// 	}
// 	if (chdir(path) != 0)
// 	{
// 		perror("cd");
// 		return (1);
// 	}
// 	return (0);
// }

void	set_env_value(t_env *env, const char *key, const char *value)
{
    if (find_and_update(env, ft_strdup(key), value))
        return;
    // If not found, add new variable
    t_env *last = env;
    while (last && last->next)
        last = last->next;
    t_env *new = malloc(sizeof(t_env));
    if (!new)
        return;
    new->key = ft_strdup(key);
    new->value = ft_strdup(value);
    new->next = NULL;
    if (last)
        last->next = new;
}

static int	update_env_pwd(t_env *env, const char *oldpwd)
{
	char	*newpwd = getcwd(NULL, 0);

	if (!newpwd)
	{
		perror("getcwd");
		return (1);
	}
	set_env_value(env, "OLDPWD", oldpwd);
	set_env_value(env, "PWD", newpwd);
	free(newpwd);
	return (0);
}

int	ft_cd(t_cmd *cmd, t_env *env)
{
	char	*path;
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("getcwd");
		oldpwd = ft_strdup("");
		if (!oldpwd)
			return (1);
	}
	path = cmd->args[1];
	if (!path || strcmp(path, "~") == 0)
	{
		path = get_env_value(env, "HOME");
		if (!path)
		{
			write(2, "cd: HOME not set\n", 17);
			free(oldpwd);
			return (1);
		}
	}
	else if (strcmp(path, "-") == 0)
	{
		path = get_env_value(env, "OLDPWD");
		if (!path)
		{
			write(2, "cd: OLDPWD not set\n", 20);
			free(oldpwd);
			return (1);
		}
		printf("%s\n", path);
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		free(oldpwd);
		return (1);
	}
	update_env_pwd(env, oldpwd);
	free(oldpwd);
	return (0);
}
