/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 22:25:33 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/22 22:26:42 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned int	ft_atoi(char *str)
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

void	*error(char *str, void *ptr)
{
	printf("%s\n", str);
	return (ptr);
}

void	*ft_exit(void *ptr)
{
	if (ptr)
		free(ptr);
	return (NULL);
}

void	ft_print(char *str, char *color, philo_t *philo)
{
	pthread_mutex_lock(&philo->vars->log_mutex);
	if (!philo->vars->simulation_end)
		printf("%s%ld %d %s\n", color, passedTime(philo->vars->start_time),
			philo->id, str);
	pthread_mutex_unlock(&philo->vars->log_mutex);
}
