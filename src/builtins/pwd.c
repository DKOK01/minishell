/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 09:58:42 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/21 07:33:44 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_pwd(t_env *env)
{
	char	*cwd;

	cwd = get_env_value(env, "PWD");
	if (cwd && *cwd)
	{
		printf("%s\n", cwd);
		return (0);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		write(2, "pwd: error retrieving current directory\n", 40);
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}
