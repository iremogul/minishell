/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iogul <iogul@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 19:08:05 by iogul             #+#    #+#             */
/*   Updated: 2026/03/09 15:08:17 by iogul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include "minishell.h"

static int	countdigit(int nbr)
{
	int	digit;

	digit = 1;
	if (nbr == 0)
		return (1);
	if (nbr < 0)
	{
		digit = digit + 1;
		nbr = -nbr;
	}
	while (nbr / 10 != 0)
	{
		nbr /= 10;
		digit++;
	}
	return (digit);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len_digit;
	long	nb;

	nb = n;
	len_digit = countdigit(nb);
	str = (char *)gc_malloc(sizeof(char) * (len_digit + 1));
	str[len_digit] = '\0';
	if (nb == 0)
		str[0] = '0';
	if (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	while (nb > 0)
	{
		str[--len_digit] = (nb % 10) + '0';
		nb /= 10;
	}
	return (str);
}
