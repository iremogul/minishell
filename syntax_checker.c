/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 19:37:23 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 12:53:56 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

static int	print_syntax_error(char *token_value, int *exit_status)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(token_value, 2);
	ft_putstr_fd("'\n", 2);
	*exit_status = 2;
	return (1);
}

static int	check_redirect_syntax(t_token *tmp, int *exit_status)
{
	if (tmp->type == INPUT || tmp->type == TRUNC
		|| tmp->type == APPEND || tmp->type == HEREDOC)
	{
		if (!tmp->next)
			return (print_syntax_error("newline", exit_status));
		if (tmp->next->type != WORD)
			return (print_syntax_error(tmp->next->value, exit_status));
	}
	return (0);
}

int	check_syntax(t_token *tokens, int *exit_status)
{
	t_token	*tmp;

	tmp = tokens;
	if (!tmp)
		return (0);
	if (tmp->type == PIPE)
		return (print_syntax_error(tmp->value, exit_status));
	while (tmp)
	{
		if (check_redirect_syntax(tmp, exit_status))
			return (1);
		if (tmp->type == PIPE)
		{
			if (!tmp->next)
				return (print_syntax_error("newline", exit_status));
			if (tmp->next->type == PIPE)
				return (print_syntax_error(tmp->next->value, exit_status));
		}
		tmp = tmp->next;
	}
	return (0);
}
