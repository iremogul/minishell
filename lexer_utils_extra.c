/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_extra.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 13:47:35 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 12:37:43 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include "libft/libft.h"

int	check_unclosed_quotes(char *input, int *exit_status)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (input[i])
	{
		if ((input[i] == '\'' || input[i] == '\"') && quote == 0)
			quote = input[i];
		else if (input[i] == quote)
			quote = 0;
		i++;
	}
	if (quote != 0)
	{
		write(2, "minishell: syntax error: unclosed quotes\n", 41);
		*exit_status = 2;
		return (1);
	}
	return (0);
}

int	handle_operator(char *input, int i, t_token **tokens)
{
	if (input[i] == '>' && input[i + 1] == '>')
	{
		ft_token_add_back(tokens, ft_token_new(ft_strdup(">>"), APPEND));
		return (i + 2);
	}
	if (input[i] == '<' && input[i + 1] == '<')
	{
		ft_token_add_back(tokens, ft_token_new(ft_strdup("<<"), HEREDOC));
		return (i + 2);
	}
	if (input[i] == '>')
		ft_token_add_back(tokens, ft_token_new(ft_strdup(">"), TRUNC));
	else if (input[i] == '<')
		ft_token_add_back(tokens, ft_token_new(ft_strdup("<"), INPUT));
	else if (input[i] == '|')
		ft_token_add_back(tokens, ft_token_new(ft_strdup("|"), PIPE));
	return (i + 1);
}

int	handle_quotes(char *input, int i, t_token **tokens)
{
	int		start;
	char	quote;
	t_token	*new;

	quote = input[i];
	start = i++;
	while (input[i] && input[i] != quote)
		i++;
	if (input[i])
		i++;
	new = ft_token_new(ft_substr(input, start, i - start), WORD);
	new->is_expandable = 0;
	ft_token_add_back(tokens, new);
	return (i);
}

int	handle_word(char *input, int i, t_token **tokens)
{
	int		start;
	t_token	*new;

	start = i;
	while (input[i] && !ft_strchr(" \t\n\v\f\r|<>\"\'", input[i]))
		i++;
	new = ft_token_new(ft_substr(input, start, i - start), WORD);
	new->is_expandable = 1;
	ft_token_add_back(tokens, new);
	return (i);
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}
