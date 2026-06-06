/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_extra.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:57:31 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 12:21:38 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <unistd.h>

int	count_env_vars(t_env *envp)
{
	int	i;

	i = 0;
	while (envp)
	{
		i++;
		envp = envp->next;
	}
	return (i);
}

void	add_new_env_helper(t_env **env, char *key, char *new_val)
{
	char	*full;
	char	*full_val;
	t_env	*new_node;
	t_env	*tmp;

	full = ft_strjoin(key, "=");
	full_val = ft_strjoin(full, new_val);
	new_node = new_env_node(full_val);
	if (*env == NULL)
	{
		*env = new_node;
		return ;
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

void	init_shell_env(t_env **env_list)
{
	char	cwd[1024];
	char	*shlvl_str;
	int		shlvl;
	char	*unset_args[3];

	if (getcwd(cwd, 1024))
		update_env_val(env_list, "PWD", cwd);
	shlvl_str = get_env_value("SHLVL", *env_list);
	if (!shlvl_str || shlvl_str[0] == '\0')
		shlvl = 1;
	else
		shlvl = ft_atoi(shlvl_str) + 1;
	update_env_val(env_list, "SHLVL", ft_itoa(shlvl));
	unset_args[0] = "unset";
	unset_args[1] = "OLDPWD";
	unset_args[2] = NULL;
	ft_unset(unset_args, env_list);
}

char	**env_list_to_array(t_env *envp)
{
	char	**env_array;
	t_env	*tmp;
	int		i;
	char	*temp_str;

	env_array = gc_malloc(sizeof(char *) * (count_env_vars(envp) + 1));
	if (!env_array)
		return (NULL);
	tmp = envp;
	i = 0;
	while (tmp)
	{
		temp_str = ft_strjoin(tmp->key, "=");
		env_array[i] = ft_strjoin(temp_str, tmp->value);
		tmp = tmp->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}
