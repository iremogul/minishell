/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 13:12:06 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 12:58:44 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include "libft/libft.h"

int	exec_single_builtin(t_cmd *cmd, t_env *env, int *lst_sts)
{
	int	stdin_backup;
	int	stdout_backup;
	int	status;

	stdin_backup = dup(0);
	stdout_backup = dup(1);
	if (setup_file_descriptors(cmd) != -1)
		status = exec_builtin(cmd, env, lst_sts);
	else
		status = 1;
	dup2(stdin_backup, 0);
	dup2(stdout_backup, 1);
	close(stdin_backup);
	close(stdout_backup);
	return (status);
}

void	handle_exec_errors(char *arg, char *full_path)
{
	if (!full_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd(": command not found", 2);
		gc_free_all();
		exit(127);
	}
	if (access(full_path, F_OK) == 0 && access(full_path, X_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd(": Permission denied", 2);
		gc_free_all();
		exit(126);
	}
	if (access(full_path, F_OK) != 0 && ft_strchr(arg, '/'))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd(": No such file or directory", 2);
		gc_free_all();
		exit(127);
	}
}
