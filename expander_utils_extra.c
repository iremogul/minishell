/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_extra.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 13:36:46 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 12:24:18 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

char	*remove_quotes(char *str)
{
	char	*new_str;
	int		i;
	int		j;
	char	quote;

	if (!str)
		return (NULL);
	new_str = gc_malloc(ft_strlen(str) + 1);
	i = 0;
	j = 0;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && quote == 0)
			quote = str[i++];
		else if (str[i] == quote && quote != 0)
		{
			quote = 0;
			i++;
		}
		else
			new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	return (new_str);
}

static int	get_word_end(char *str, int i)
{
	char	q;

	q = 0;
	while (str[i] && !((str[i] == ' ' && q == 0)))
	{
		if ((str[i] == '\'' || str[i] == '\"') && q == 0)
			q = str[i];
		else if (str[i] == q)
			q = 0;
		i++;
	}
	return (i);
}

int	count_unquoted_words(char *str)
{
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (!str[i])
			break ;
		count++;
		i = get_word_end(str, i);
	}
	return (count);
}

char	**split_unquoted(char *str)
{
	char	**arr;
	int		i;
	int		j;
	int		start;

	arr = gc_malloc(sizeof(char *) * (count_unquoted_words(str) + 1));
	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (!str[i])
			break ;
		start = i;
		i = get_word_end(str, i);
		arr[j++] = ft_substr(str, start, i - start);
	}
	arr[j] = NULL;
	return (arr);
}
