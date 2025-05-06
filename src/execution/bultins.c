/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultins.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysadeq <aysadeq@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 10:46:14 by ael-mans          #+#    #+#             */
/*   Updated: 2025/05/06 08:30:07 by aysadeq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_echo(t_cmd *cmd)
{
	int i = 1;
	int newline = 1;

	if (cmd->args[1] && ft_strcmp(cmd->args[1], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (cmd->args[i])
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

int ft_cd(t_cmd *cmd)
{
	char *path;

	path = cmd->args[1];
	if (!path)
	{
		path = getenv("HOME");
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

int ft_exit(t_cmd *cmd)
{
	int status;

	status = 0;
	if (cmd->args[1])
	{
		status = ft_atoi(cmd->args[1]);
		if (status < 0)
			status = 256 + status;
	}
	exit(status);
}
