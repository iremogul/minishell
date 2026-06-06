/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 13:52:11 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 12:52:31 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

static void	init_cmd_node(t_cmd *cmd, int arg_count)
{
	cmd->args = gc_malloc(sizeof(char *) * (arg_count + 1));
	cmd->cmd = NULL;
	cmd->redirs = NULL;
	cmd->infile = 0;
	cmd->outfile = 1;
	cmd->prev = NULL;
	cmd->next = NULL;
}

static void	process_token(t_cmd *cmd, t_token **curr_token, int *i)
{
	char	*cleaned;

	if ((*curr_token)->type == WORD)
	{
		cleaned = ft_strdup((*curr_token)->value);
		if (*i == 0)
			cmd->cmd = ft_strdup(cleaned);
		cmd->args[(*i)++] = cleaned;
	}
	else
		handle_redirection(cmd, curr_token);
}

t_cmd	*parse_single_command(t_token **curr_token)
{
	t_cmd	*cmd;
	int		arg_count;
	int		i;

	cmd = gc_malloc(sizeof(t_cmd));
	arg_count = count_args(*curr_token);
	init_cmd_node(cmd, arg_count);
	i = 0;
	while (*curr_token && (*curr_token)->type != PIPE)
	{
		process_token(cmd, curr_token, &i);
		if (*curr_token && (*curr_token)->type != PIPE)
			*curr_token = (*curr_token)->next;
	}
	cmd->args[i] = NULL;
	return (cmd);
}

t_cmd	*parser(t_token *tokens, int *exit_status)
{
	t_cmd	*head;
	t_cmd	*curr;

	if (!tokens || check_syntax(tokens, exit_status))
		return (NULL);
	head = parse_single_command(&tokens);
	curr = head;
	while (tokens && tokens->type == PIPE)
	{
		tokens = tokens->next;
		curr->next = parse_single_command(&tokens);
		curr->next->prev = curr;
		curr = curr->next;
	}
	curr->next = NULL;
	return (head);
}
