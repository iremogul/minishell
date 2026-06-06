/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 19:50:19 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 12:22:58 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

char	*get_env_value(char *key, t_env *env_list)
{
	while (env_list)
	{
		if (ft_strncmp(env_list->key, key, ft_strlen(key) + 1) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return ("");
}

t_env	*new_env_node(char *env_str)
{
	t_env	*new;
	char	*sign;

	new = (t_env *)gc_malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	sign = ft_strchr(env_str, '=');
	if (sign)
	{
		new->key = ft_substr(env_str, 0, sign - env_str);
		new->value = ft_strdup(sign + 1);
	}
	else
	{
		new->key = ft_strdup(env_str);
		new->value = NULL;
	}
	new->next = NULL;
	return (new);
}

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*current;
	int		i;

	if (!envp || !envp[0])
		return (NULL);
	head = new_env_node(envp[0]);
	current = head;
	i = 1;
	while (envp[i])
	{
		current->next = new_env_node(envp[i]);
		current = current->next;
		i++;
	}
	return (head);
}

t_env	*find_env_node(t_env *lst_env, char *search_key)
{
	t_env	*current_node;

	current_node = lst_env;
	while (current_node != NULL)
	{
		if (ft_strncmp(current_node->key, search_key, \
		ft_strlen(search_key) + 1) == 0)
			return (current_node);
		current_node = current_node->next;
	}
	return (NULL);
}

void	update_env_val(t_env **env, char *key, char *new_val)
{
	t_env	*tmp_node;

	tmp_node = find_env_node(*env, key);
	if (tmp_node != NULL)
		tmp_node->value = ft_strdup(new_val);
	else
		add_new_env_helper(env, key, new_val);
}
