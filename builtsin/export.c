/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 15:27:12 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 12:28:02 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

static int	is_valid_id(char *str)
{
	int	i;

	i = 0;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	sort_export_arr(char **arr, int size)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < size - 1)
	{
		j = i + 1;
		while (j < size)
		{
			if (ft_strncmp(arr[i], arr[j], ft_strlen(arr[i]) + 1) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_sorted_export(t_env *env)
{
	char	**arr;
	int		i;
	t_env	*node;

	arr = gc_malloc(sizeof(char *) * (count_env_vars(env) + 1));
	i = 0;
	node = env;
	while (node)
	{
		if (ft_strncmp(node->key, "_", 2) != 0)
			arr[i++] = node->key;
		node = node->next;
	}
	arr[i] = NULL;
	sort_export_arr(arr, i);
	i = -1;
	while (arr[++i])
	{
		node = find_env_node(env, arr[i]);
		(ft_putstr_fd("declare -x ", 1), ft_putstr_fd(node->key, 1));
		if (node->value)
			(ft_putstr_fd("=\"", 1), ft_putstr_fd(node->value, 1),
				ft_putstr_fd("\"", 1));
		ft_putstr_fd("\n", 1);
	}
}

static void	process_export_arg(t_env *env, char *arg)
{
	char	*key;
	char	*sign;
	t_env	*node;

	sign = ft_strchr(arg, '=');
	if (sign)
		key = ft_substr(arg, 0, sign - arg);
	else
		key = ft_strdup(arg);
	node = find_env_node(env, key);
	if (node && sign)
		update_env_val(&env, key, ft_strdup(sign + 1));
	else if (!node)
	{
		while (env && env->next)
			env = env->next;
		if (env)
			env->next = new_env_node(arg);
	}
}

int	ft_export(char **args, t_env *env)
{
	int	i;
	int	exit_status;

	exit_status = 0;
	if (!args[1])
	{
		print_sorted_export(env);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (!is_valid_id(args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			exit_status = 1;
		}
		else
			process_export_arg(env, args[i]);
		i++;
	}
	return (exit_status);
}
