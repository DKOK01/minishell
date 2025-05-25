/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mans <ael-mans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 00:46:16 by ael-mans          #+#    #+#             */
/*   Updated: 2025/05/25 00:52:14 by ael-mans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	setup_child_pipe(t_cmd *cmd, int prev_fd, int *pipe_fd, t_env *env)
{
    if (prev_fd != -1)
    {
        dup2(prev_fd, STDIN_FILENO);
        close(prev_fd);
    }
    if (cmd->next)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
    }
    if (cmd->infile || cmd->append || cmd->heredoc || cmd->outfile)
        check_redirection(cmd);
    if (check_builtins(cmd))
        run_builtin(cmd, env);
    else
        execute_command(cmd, env);
    exit(0);
}

int	handle_pipeline(t_cmd *cmd, t_env *env)
{
    int		pipe_fd[2];
    int		prev_fd = -1;
    pid_t	pid;

    while (cmd)
    {
        if (cmd->next && pipe(pipe_fd) < 0)
        {
            perror("pipe");
            exit(1);
        }
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(1);
        }
        if (pid == 0)
            setup_child_pipe(cmd, prev_fd, pipe_fd, env);
        if (prev_fd != -1)
            close(prev_fd);
        if (cmd->next)
        {
            close(pipe_fd[1]);
            prev_fd = pipe_fd[0];
        }
        cmd = cmd->next;
    }
    while (wait(NULL) > 0)
        ;
    return (0);
}
