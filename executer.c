/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 13:10:09 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 12:58:31 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h> 
#include <stdio.h>
#include "libft/libft.h"

static void	exec_cmd(t_cmd *tmp, t_env *env)
{
	char	*full_path;
	char	**c_env;

	if (!tmp->args || !tmp->args[0])
		exit(0);
	if (ft_strchr(tmp->args[0], '/'))
		full_path = ft_strdup(tmp->args[0]);
	else
		full_path = find_path(tmp->args[0], get_paths_from_env(env));
	if (full_path)
		update_env_val(&env, "_", full_path);
	c_env = env_list_to_array(env);
	handle_exec_errors(tmp->args[0], full_path);
	execve(full_path, tmp->args, c_env);
	ft_putstr_fd("minishell: ", 2);
	perror(tmp->args[0]);
	gc_free_all();
	exit(126);
}

static void	setup_child_streams(t_cmd *tmp, int *fd, int fd_in)
{
	if (fd_in != 0)
	{
		dup2(fd_in, 0);
		close(fd_in);
	}
	if (tmp->next)
	{
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
	}
	if (setup_file_descriptors(tmp) == -1)
	{
		gc_free_all();
		exit(1);
	}
}

static void	child_process(t_cmd *tmp, int *fd, int fd_in, t_env *env)
{
	int	b_status;
	int	dummy_status;

	dummy_status = 0;
	reset_signals();
	setup_child_streams(tmp, fd, fd_in);
	if (is_builtin(tmp->args[0]))
	{
		b_status = exec_builtin(tmp, env, &dummy_status);
		gc_free_all();
		exit(b_status);
	}
	exec_cmd(tmp, env);
}

static pid_t	handle_fork(t_cmd *tmp, int *fd, int *fd_in, t_env *env)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		child_process(tmp, fd, *fd_in, env);
	if (*fd_in != 0)
		close(*fd_in);
	if (tmp->next)
	{
		*fd_in = fd[0];
		close(fd[1]);
	}
	return (pid);
}

int	executer(t_cmd *cmd, t_env *env, int lst_sts)
{
	int		fd[2];
	int		fd_in;
	t_cmd	*tmp;
	pid_t	last_pid;

	if (check_heredocs(cmd, env, lst_sts) == 1)
		return (130);
	ignore_signal();
	fd_in = 0;
	tmp = cmd;
	last_pid = -1;
	while (tmp)
	{
		if (tmp->args && tmp->args[0])
			update_env_val(&env, "_", tmp->args[0]);
		if (tmp->next && pipe(fd) == -1)
			exit(1);
		if (!tmp->next && !tmp->prev && is_builtin(tmp->args[0]))
			lst_sts = exec_single_builtin(tmp, env, &lst_sts);
		else
			last_pid = handle_fork(tmp, fd, &fd_in, env);
		tmp = tmp->next;
	}
	return (finish_execution(cmd, lst_sts, last_pid));
}
