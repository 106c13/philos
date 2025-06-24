/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:23:16 by haaghaja          #+#    #+#             */
/*   Updated: 2025/06/24 14:35:41 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_int(char *num)
{
	int	i;

	i = 0;
	while (num[i])
	{
		if (num[i] < '0' || num[i] > '9')
			return (0);
		i++;
	}
	if (i > 0 && i < 12)
		return (1);
	return (0);
}

int	is_valid(int argc, char **argv)
{
	int	i;
	int	val;

	i = 1;
	while (i < argc)
	{
		if (!is_int(argv[i]))
			return (0);
		val = ft_atoi(argv[i]);
		if (val <= 0 && (i != 5 && i != 1))
			return (0);
		i++;
	}
	return (1);
}
