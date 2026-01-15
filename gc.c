/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iogul <iogul@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 20:51:32 by iogul             #+#    #+#             */
/*   Updated: 2026/01/15 20:51:33 by iogul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"

t_gc	**get_gc_list(void)
{
	static t_gc	*head = NULL;

	return (&head);
}

void	gc_add_node(void *ptr)
{
	t_gc	**head;
	t_gc	*new_node;

	head = get_gc_list();
	new_node = malloc(sizeof(t_gc));
	if (!new_node)
	{
		free(ptr);
		exit(1);
	}
	new_node->ptr = ptr;
	new_node->next = *head;
	*head = new_node;
}

void	*gc_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	gc_add_node(ptr);
	return (ptr);
}

void	gc_free_all(void)
{
	t_gc	**head;
	t_gc	*current;
	t_gc	*next_node;

	head = get_gc_list();
	current = *head;
	while (current)
	{
		next_node = current->next;
		if (current->ptr)
			free(current->ptr);
		free(current);
		current = next_node;
	}
	*head = NULL;
}
