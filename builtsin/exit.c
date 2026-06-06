/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 14:58:23 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 13:04:43 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft/libft.h"

static int	check_overflow(unsigned long long acc, int next_digit, int sign)
{
	if (sign == 1)
	{
		if (acc > LLONG_MAX / 10 || (acc == LLONG_MAX / 10 && next_digit > 7))
			return (1);
	}
	else
	{
		if (acc > (unsigned long long)LLONG_MAX / 10
			|| (acc == (unsigned long long)LLONG_MAX / 10 && next_digit > 8))
			return (1);
	}
	return (0);
}

static int	ft_atoll_safe(const char *str, long long *res)
{
	unsigned long long	acc;
	int					i;
	int					sign;

	acc = 0;
	i = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if ((str[i] == '-' || str[i] == '+') && str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]) || check_overflow(acc, str[i] - '0', sign))
			return (0);
		acc = acc * 10 + (str[i++] - '0');
	}
	*res = acc * sign;
	return (1);
}

static void	exit_error_numeric(char *arg)
{
	int	i;

	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": numeric argument required", 2);
	gc_free_all();
	i = 2;
	while (++i < 1024)
		close(i);
	exit(2);
}

static void	close_fds(void)
{
	int	i;

	i = 2;
	while (++i < 1024)
		close(i);
}

int	ft_exit(char **args, int *last_exit_status)
{
	long long	res;

	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", 1);
	if (!args[1])
	{
		gc_free_all();
		close_fds();
		exit(*last_exit_status);
	}
	if (!ft_atoll_safe(args[1], &res))
		exit_error_numeric(args[1]);
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		*last_exit_status = 1;
		return (1);
	}
	gc_free_all();
	close_fds();
	exit((unsigned char)res);
}
