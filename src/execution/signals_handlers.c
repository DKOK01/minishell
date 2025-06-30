/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handlers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:20:00 by ael-mans          #+#    #+#             */
/*   Updated: 2025/06/30 17:20:14 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	sigint_handler(int sig)
{
	(void)sig;
	g_exit_status = 130;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	exec_sigint_handler(int sig)
{
	(void)sig;
	g_exit_status = 130;
	write(1, "\n", 1);
}

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_exit_status = 130;
	write(1, "\n", 1);
	exit(130);
}
