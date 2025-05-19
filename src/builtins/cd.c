/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 09:58:09 by ael-mans          #+#    #+#             */
/*   Updated: 2025/05/19 12:53:20 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_cd(t_cmd *cmd, t_env *env)
{
	char	*path;

	path = cmd->args[1];
	if (!path)
	{
		path = get_env_value(env, "HOME");
		if (!path)
		{
			write(2, "cd: HOME not set\n", 17);
			return (1);
		}
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}
