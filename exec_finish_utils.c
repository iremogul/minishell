/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_finish_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 13:17:12 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 11:56:12 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

static int	wait_and_get_status(pid_t last_pid, int *lst_sts)
{
	int		status;
	pid_t	pid;

	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				*lst_sts = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				*lst_sts = 128 + WTERMSIG(status);
				if (WTERMSIG(status) == SIGINT)
					write(1, "\n", 1);
				else if (WTERMSIG(status) == SIGQUIT)
					write(1, "Quit (core dumped)\n", 19);
			}
		}
		pid = waitpid(-1, &status, 0);
	}
	return (*lst_sts);
}

int	finish_execution(t_cmd *cmd, int lst_sts, pid_t last_pid)
{
	lst_sts = wait_and_get_status(last_pid, &lst_sts);
	init_signals();
	unlink_heredocs(cmd);
	return (lst_sts);
}
