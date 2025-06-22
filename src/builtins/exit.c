/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 09:59:09 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/22 10:12:24 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	cleanup_resources(t_env **env)
{
	if (env && *env)
	{
		free_env_list(*env);
		*env = NULL;
	}
	clear_history();
}

int	ft_exit(t_cmd *cmd, t_env **env)
{
	int	status;

	printf("exit\n");
	if (!cmd->args[1])
	{
		cleanup_resources(env);
		exit(0);
	}
	if (!is_numeric(cmd->args[1]))
	{
		write(2, "exit: ", 6);
		write(2, cmd->args[1], ft_strlen(cmd->args[1]));
		write(2, ": numeric argument required\n", 28);
		cleanup_resources(env);
		exit(2);
	}
	if (cmd->args[2])
	{
		write(2, "exit: too many arguments\n", 25);
		return (1);
	}
	status = ft_atoi(cmd->args[1]);
	cleanup_resources(env);
	exit(status & 255);
}
