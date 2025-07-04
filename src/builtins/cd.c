/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 09:58:09 by ael-mans          #+#    #+#             */
/*   Updated: 2025/07/04 14:42:20 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*create_node(const char *key, const char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	return (new_node);
}

void	set_env_value(t_env **env, const char *key, const char *value)
{
	t_env	*current;
	t_env	*last;
	t_env	*new_node;

	current = *env;
	last = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		last = current;
		current = current->next;
	}
	new_node = create_node(key, value);
	if (!new_node)
		return ;
	if (!*env)
		*env = new_node;
	else
		last->next = new_node;
}

static int	update_env_pwd(t_env **env, const char *oldpwd)
{
	char	*newpwd;

	newpwd = getcwd(NULL, 0);
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

static int	handle_cd_path(t_cmd *cmd, t_env **env, char **path)
{
	*path = cmd->args[1];
	if (!*path || ft_strcmp(*path, "~") == 0)
	{
		*path = get_env_value(*env, "HOME");
		if (!*path || !**path)
		{
			write(2, "cd: HOME not set\n", 17);
			return (1);
		}
	}
	else if (ft_strcmp(*path, "-") == 0)
	{
		*path = get_env_value(*env, "OLDPWD");
		if (!*path || !**path)
		{
			write(2, "cd: OLDPWD not set\n", 20);
			return (1);
		}
		printf("%s\n", *path);
	}
	return (0);
}

int	ft_cd(t_cmd *cmd, t_env **env)
{
	char	*path;
	char	*oldpwd;

	if (cmd->args[1] && cmd->args[2])
	{
		write(2, "cd: too many arguments\n", 23);
		return (1);
	}
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("getcwd");
		oldpwd = ft_strdup("");
		if (!oldpwd)
			return (1);
	}
	if (handle_cd_path(cmd, env, &path) != 0)
		return (free(oldpwd), 1);
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: No such file or directory\n", 2);
		return (free(oldpwd), 1);
	}
	update_env_pwd(env, oldpwd);
	return (free(oldpwd), 0);
}
