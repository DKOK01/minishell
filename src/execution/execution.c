/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 08:46:48 by ael-mans          #+#    #+#             */
/*   Updated: 2025/05/06 12:44:02 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	run_builtin(t_cmd *cmd, t_env *env)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (ft_echo(cmd));
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (ft_cd(cmd, env));
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (ft_pwd());
	// else if (ft_strcmp(cmd->args[0], "export") == 0)
	// 	return (ft_export(cmd));
	// else if (ft_strcmp(cmd->args[0], "unset") == 0)
	// 	return (ft_unset(cmd));
	// else if (ft_strcmp(cmd->args[0], "env") == 0)
	// 	return (ft_env());
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (ft_exit(cmd));
	return (-1);
}