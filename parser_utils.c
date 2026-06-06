/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 13:52:28 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 12:51:50 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

void	merge_tokens(t_token *tokens)
{
	t_token	*curr;
	t_token	*tmp;

	curr = tokens;
	while (curr && curr->next)
	{
		if (curr->type == WORD && curr->next->type == WORD)
		{
			curr->value = ft_strjoin(curr->value, curr->next->value);
			if (curr->next->is_expandable == 0)
				curr->is_expandable = 0;
			tmp = curr->next;
			curr->next = tmp->next;
			if (tmp->next)
				tmp->next->prev = curr;
			continue ;
		}
		curr = curr->next;
	}
}

int	count_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != PIPE)
	{
		if (tokens->type == WORD)
		{
			count++;
			tokens = tokens->next;
		}
		else
		{
			tokens = tokens->next;
			if (tokens)
				tokens = tokens->next;
		}
	}
	return (count);
}

static void	add_redir_node(t_cmd *cmd, char *file, t_redir_type type, \
	int is_exp)
{
	t_redir	*new;
	t_redir	*tmp;

	new = gc_malloc(sizeof(t_redir));
	new->type = type;
	new->file = ft_strdup(file);
	new->is_expandable = is_exp;
	new->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = new;
	else
	{
		tmp = cmd->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	handle_redirection(t_cmd *cmd, t_token **curr_token)
{
	t_redir_type	type;
	t_token			*op_token;

	op_token = *curr_token;
	if (op_token->type == INPUT)
		type = R_INPUT;
	else if (op_token->type == TRUNC)
		type = R_TRUNC;
	else if (op_token->type == APPEND)
		type = R_APPEND;
	else
		type = R_HEREDOC;
	if (op_token->next && op_token->next->type == WORD)
	{
		*curr_token = op_token->next;
		add_redir_node(cmd, (*curr_token)->value, type, \
			(*curr_token)->is_expandable);
	}
}
