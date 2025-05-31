/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:52:36 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/31 20:17:37 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	join_threads(pthread_t *threads, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

int	error(char *msg)
{
	printf("%s", msg);
	return (0);
}

int	ft_usleep(long ms, t_philo *philo)
{
	long	ct;
	long	st;

	st = current_time();
	while (1)
	{
		ct = current_time();
		if (ct - philo->last_time_eat >= philo->vars->time_to_die)
		{
			philo->last_time_eat = 0;
			return (0);
		}
		if (ct - st >= ms)
			break;
		usleep(500);
	}
	return (1);
}

