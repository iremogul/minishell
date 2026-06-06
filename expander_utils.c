/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 13:36:30 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 12:29:24 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

char	*get_var_name(char *str, int *i)
{
	int		start;

	start = ++(*i);
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

char	*process_var(char *str, int *i, t_env *env_list, int status)
{
	char	*v_name;
	char	*v_val;

	v_name = get_var_name(str, i);
	if (ft_strncmp(v_name, "?", 2) == 0)
		v_val = ft_itoa(status);
	else
		v_val = get_env_value(v_name, env_list);
	return (v_val);
}

static char	*append_char(char *new_str, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	return (ft_strjoin(new_str, tmp));
}

char	*expand_string(char *str, t_env *env_list, int last_exit_status)
{
	char	*new_str;
	int		i;
	char	q;

	new_str = ft_strdup("");
	i = 0;
	q = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && q == 0)
			q = str[i];
		else if (str[i] == q)
			q = 0;
		if (q != '\'' && str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) \
			|| str[i + 1] == '_' || str[i + 1] == '?'))
			new_str = ft_strjoin(new_str, process_var(str, &i, env_list, \
			last_exit_status));
		else
			new_str = append_char(new_str, str[i++]);
	}
	return (new_str);
}
