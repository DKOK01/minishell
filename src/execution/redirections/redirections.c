/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 13:38:30 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/22 16:29:44 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	check_redirection(t_cmd *cmd)
{
	if (cmd->infile && cmd->heredoc == 0)
		handle_infile(cmd);
	if (cmd->heredoc && cmd->heredoc_fd != -1)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	if (cmd->outfile)
		handle_outfile(cmd);
	return (0);
}
