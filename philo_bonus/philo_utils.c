/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 19:02:43 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/28 17:20:29 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	t_philo_take_forks(t_philo *philo)
{
	printf("====\n");
	sem_wait(philo->forks);
	ft_print("has taken a fork", WHITE, philo);
	if (philo->vars->num == 1)
	{
		usleep(philo->vars->time_to_die);
		sem_post(philo->forks);
		return (0);
	}
	sem_wait(philo->forks);
	ft_print("has taken a fork", WHITE, philo);
	return (1);
}

int	philo_eat(t_philo *philo)
{
	long	ct;

	if (philo->vars->number_of_meals == 0)
		return (0);
	if (!t_philo_take_forks(philo))
		return (0);
	ct = current_time();
	if (ct - philo->last_time_eat > philo->vars->time_to_die)
	{
		ft_print("is dead", DARK, philo);
		sem_post(philo->forks);
		sem_post(philo->forks);
		return (0);
	}
	ft_print("is eating", RED, philo);
	usleep(philo->vars->time_to_eat * 1000);
	philo->last_time_eat = current_time();
	sem_post(philo->forks);
	sem_post(philo->forks);
	return (1);
}

int	philo_sleep(t_philo *philo)
{
	ft_print("is sleeping", BLUE, philo);
	usleep(philo->vars->time_to_sleep * 1000);
	return (1);
}

void	simulation(t_philo *philo)
{
	if (philo->id % 2 == 0)
		usleep(500);
	while (!philo->vars->simulation_end)
	{
		ft_print("is thinking", GREEN, philo);
		if (!philo_eat(philo))
			break ;
		if (!philo_sleep(philo))
			break ;
	}
}
