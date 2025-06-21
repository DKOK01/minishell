/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 13:38:30 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/21 07:33:53 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_redirection(t_cmd *cmd)
{
	if (cmd->infile && cmd->heredoc == 0)
		handle_infile(cmd);
	if (cmd->heredoc)
		handle_heredoc(cmd);
	if (cmd->outfile)
		handle_outfile(cmd);
	return (0);
}
