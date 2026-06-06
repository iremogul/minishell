/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iogul <iogul@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:11:54 by iogul             #+#    #+#             */
/*   Updated: 2026/02/24 16:44:56 by iogul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include "minishell.h"

char	*ft_strdup(const char *s)
{
	char	*s2;
	char	*ptr;
	size_t	len;

	len = ft_strlen(s);
	s2 = (char *)gc_malloc((len + 1) * sizeof(char));
	ptr = s2;
	while (*s != '\0')
	{
		*s2 = *s;
		++s2;
		++s;
	}
	*s2 = '\0';
	return (ptr);
}
