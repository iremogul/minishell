/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 13:16:44 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 13:56:54 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include "libft/libft.h"
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h> 

int	g_heredoc_sig = 0;

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_heredoc_sig = 1;
	close(STDIN_FILENO);
}

static void	get_heredoc_name(char *buffer, int index)
{
	char	*num;
	char	*name;

	num = ft_itoa(index);
	name = ft_strjoin(".heredoc_", num);
	ft_strlcpy(buffer, name, 255);
}

static void	heredoc_child(t_redir *node, t_env *env, int lst_sts, int fd)
{
	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	heredoc_loop(node, env, lst_sts, fd);
	gc_free_all();
	close(fd);
	if (g_heredoc_sig == 1)
		exit(130);
	exit(0);
}

int	run_heredoc(t_redir *node, t_env *env, int index, int lst_sts)
{
	int		fd;
	pid_t	pid;
	int		status;
	char	fname[255];

	get_heredoc_name(fname, index);
	fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	ignore_signal();
	pid = fork();
	if (pid == -1)
	{
		close(fd);
		return (-1);
	}
	if (pid == 0)
		heredoc_child(node, env, lst_sts, fd);
	waitpid(pid, &status, 0);
	close(fd);
	init_signals();
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		return (write(1, "\n", 1), -2);
	node->file = ft_strdup(fname);
	return (0);
}
