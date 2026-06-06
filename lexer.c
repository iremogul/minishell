/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 13:45:31 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 12:44:02 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

static int	handle_word_append(char *input, int i, t_token *tokens)
{
	int		start;
	char	q;
	char	*sub;

	start = i;
	if (input[i] == '\'' || input[i] == '\"')
	{
		q = input[i++];
		while (input[i] && input[i] != q)
			i++;
		if (input[i])
			i++;
	}
	else
	{
		while (input[i] && !ft_strchr(" \t\n\v\f\r|<>\"\'", input[i]))
			i++;
	}
	sub = ft_substr(input, start, i - start);
	tokens->value = ft_strjoin(tokens->value, sub);
	return (i);
}

static int	lexer_internal(char *input, int i, t_token **tokens)
{
	int	had_space;

	had_space = 0;
	while (input[i] && ft_strchr(" \t\n\v\f\r", input[i]))
	{
		i++;
		had_space = 1;
	}
	if (!input[i])
		return (i);
	if (had_space == 0 && *tokens && ft_token_last(*tokens)->type == WORD \
		&& !is_operator(input[i]))
		i = handle_word_append(input, i, ft_token_last(*tokens));
	else if (input[i] == '\'' || input[i] == '\"')
		i = handle_quotes(input, i, tokens);
	else if (is_operator(input[i]))
		i = handle_operator(input, i, tokens);
	else
		i = handle_word(input, i, tokens);
	return (i);
}

t_token	*lexer(char *input, int *exit_status)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	if (check_unclosed_quotes(input, exit_status))
		return (NULL);
	while (input[i])
	{
		i = lexer_internal(input, i, &tokens);
	}
	return (tokens);
}
