/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 22:25:33 by haaghaja          #+#    #+#             */
/*   Updated: 2025/06/24 15:35:48 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(char *str)
{
	unsigned int	num;

	num = 0;
	while (*str)
	{
		num = num * 10 + *str - 48;
		str++;
	}
	return (num);
}

int	is_number(char *str)
{
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

int	ft_exit(char *error_msg)
{
	printf("%s\n", error_msg);
	return (1);
}

void	ft_print(char *str, char *color, t_philo *philo)
{
	sem_wait(philo->log_sem);
	printf("%s%ld %d %s\n", color, passed_time(philo->vars->start_time),
		philo->id, str);
	sem_post(philo->log_sem);
}

int	is_time_passed(t_philo *philo)
{
	long	ct;

	ct = current_time();
	if (ct - philo->last_time_eat > philo->vars->time_to_die)
	{
		ft_print("died", DARK, philo);
		philo_exit(philo);
		return (1);
	}
	return (0);
}
