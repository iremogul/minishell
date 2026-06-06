/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 14:56:04 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 12:20:15 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

void	ft_unset(char **args, t_env **env_list)
{
	int		i;
	t_env	*curr;
	t_env	*prev;

	i = 1;
	while (args[i])
	{
		curr = *env_list;
		prev = NULL;
		while (curr)
		{
			if (ft_strncmp(curr->key, args[i], ft_strlen(args[i]) + 1) == 0)
			{
				if (prev == NULL)
					*env_list = curr->next;
				else
					prev->next = curr->next;
				break ;
			}
			prev = curr;
			curr = curr->next;
		}
		i++;
	}
}
