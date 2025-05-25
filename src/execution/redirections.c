/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 13:38:30 by ael-mans          #+#    #+#             */
/*   Updated: 2025/05/25 02:16:03 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_redirection(t_cmd *cmd)
{
	if (cmd->infile && cmd->heredoc == 0)
		handle_infile(cmd);
	else if (cmd->outfile && cmd->append == 0)
		handle_outfile(cmd);
	else if (cmd->heredoc)
		handle_heredoc(cmd);
	else if (cmd->outfile && cmd->append)
		handle_outfile(cmd);
	return (0);
}