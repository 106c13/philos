/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 19:02:34 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/24 19:02:35 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000));
}

long	passed_time(long start_time)
{
	return (current_time() - start_time);
}

void	unlock_forks(t_philo philo)
{
	int	i;

	i = 0;
	while (i < philo.vars->num)
	{
		pthread_mutex_unlock(&philo.vars->forks[i]);
		i++;
	}
}
