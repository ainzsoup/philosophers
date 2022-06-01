/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgamraou <sgamraou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 16:53:55 by sgamraou          #+#    #+#             */
/*   Updated: 2022/06/01 05:53:24 by sgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static	int	thewhitevoid(char c)
{
	if (c == ' ' || c == '\n' || c == '\t'
		|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int				i;
	unsigned long	res;
	int				sign;

	i = 0;
	res = 0;
	sign = 1;
	while (thewhitevoid(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (res > 9223372036854775807 && sign == -1)
			return (0);
		if (res >= 9223372036854775807 && sign == 1)
			return (-1);
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	return (res * sign);
}
