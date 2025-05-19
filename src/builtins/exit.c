/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 09:59:09 by ael-mans          #+#    #+#             */
/*   Updated: 2025/05/19 12:56:02 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_exit(t_cmd *cmd)
{
	int	status;

	status = 0;
	if (cmd->args[1])
	{
		status = ft_atoi(cmd->args[1]);
		if (status < 0)
			status = 256 + status;
	}
	printf("exit\n");
	exit(status);
}
