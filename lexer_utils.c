/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:08:53 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 12:43:11 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

t_token	*ft_token_last(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	while (tokens->next)
		tokens = tokens->next;
	return (tokens);
}

t_token	*ft_token_new(char *value, t_type type)
{
	t_token	*new;

	new = (t_token *)gc_malloc(sizeof(t_token));
	new->value = value;
	new->type = type;
	new->is_expandable = 1;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	ft_token_add_back(t_token **tokens, t_token *new)
{
	t_token	*tmp;

	if (!new)
		return ;
	if (!*tokens)
	{
		*tokens = new;
		return ;
	}
	tmp = *tokens;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}
