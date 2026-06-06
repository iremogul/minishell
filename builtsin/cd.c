/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 14:49:43 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 13:55:39 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <unistd.h>
#include <stdio.h>

static int	handle_home_path(t_env *env, char **path)
{
	*path = get_env_value("HOME", env);
	if (!*path || (*path)[0] == '\0')
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (1);
	}
	return (0);
}

int	ft_cd(char **args, t_env *env)
{
	char	*path;
	char	cwd[1024];

	if (args[1] && args[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		return (1);
	}
	if (!getcwd(cwd, 1024))
		return (perror("minishell: pwd"), 1);
	if (args[1])
		path = args[1];
	else if (handle_home_path(env, &path))
		return (1);
	if (chdir(path) == -1)
	{
		perror("minishell: cd");
		return (1);
	}
	update_env_val(&env, "OLDPWD", cwd);
	if (getcwd(cwd, 1024))
		update_env_val(&env, "PWD", cwd);
	return (0);
}
