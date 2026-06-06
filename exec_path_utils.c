/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 13:14:54 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 13:04:25 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include "libft/libft.h"

char	**get_paths_from_env(t_env *envp)
{
	char	*path_env;

	path_env = get_env_value("PATH", envp);
	if (!path_env)
		return (NULL);
	return (ft_split(path_env, ':'));
}

char	*find_path(char *command, char **paths)
{
	int		i;
	char	*path_part;
	char	*full_path;

	if (!paths || !command)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		path_part = ft_strjoin(paths[i], "/");
		if (!path_part)
			return (NULL);
		full_path = ft_strjoin(path_part, command);
		if (access(full_path, X_OK) == 0)
			return (full_path);
	}
	return (NULL);
}

int	is_builtin(char *command)
{
	if (!command)
		return (0);
	if (ft_strncmp(command, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(command, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(command, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(command, "export", 7) == 0)
		return (1);
	if (ft_strncmp(command, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(command, "env", 4) == 0)
		return (1);
	if (ft_strncmp(command, "exit", 5) == 0)
		return (1);
	return (0);
}

int	exec_builtin(t_cmd *tmp, t_env *env, int *last_exit_status)
{
	char	*cmd;

	if (!tmp->args || !tmp->args[0])
		return (0);
	cmd = tmp->args[0];
	if (!ft_strncmp(cmd, "cd", 3))
		return (ft_cd(tmp->args, env));
	if (!ft_strncmp(cmd, "pwd", 4))
		return (ft_pwd());
	if (!ft_strncmp(cmd, "env", 4))
		return (ft_env(env));
	if (!ft_strncmp(cmd, "echo", 5))
		return (ft_echo(tmp->args), 0);
	if (!ft_strncmp(cmd, "export", 7))
		return (ft_export(tmp->args, env));
	if (!ft_strncmp(cmd, "unset", 6))
		return (ft_unset(tmp->args, &env), 0);
	if (!ft_strncmp(cmd, "exit", 5))
		return (ft_exit(tmp->args, last_exit_status));
	return (0);
}
