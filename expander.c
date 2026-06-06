/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 13:35:55 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 12:30:19 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

static void	insert_split_tokens(t_token *tmp, char **split_vals)
{
	t_token	*next_save;
	t_token	*new_node;
	int		i;

	next_save = tmp->next;
	i = 0;
	while (split_vals[++i])
	{
		new_node = ft_token_new(remove_quotes(split_vals[i]), WORD);
		tmp->next = new_node;
		new_node->prev = tmp;
		tmp = new_node;
	}
	tmp->next = next_save;
	if (next_save)
		next_save->prev = tmp;
}

static void	handle_word_expansion(t_token *tmp, t_env *env, int status)
{
	char	*expanded;
	char	**split_vals;

	expanded = expand_string(tmp->value, env, status);
	split_vals = split_unquoted(expanded);
	if (split_vals && split_vals[0])
	{
		tmp->value = remove_quotes(split_vals[0]);
		insert_split_tokens(tmp, split_vals);
	}
	else
		tmp->value = remove_quotes(expanded);
}

void	expander(t_token *tokens, t_env *env_list, int last_exit_status)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == WORD)
		{
			if (tmp->prev && tmp->prev->type == HEREDOC)
			{
				if (ft_strchr(tmp->value, '\'') || ft_strchr(tmp->value, '\"'))
					tmp->is_expandable = 0;
				else
					tmp->is_expandable = 1;
				tmp->value = remove_quotes(tmp->value);
			}
			else
				handle_word_expansion(tmp, env_list, last_exit_status);
		}
		tmp = tmp->next;
	}
}
