/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 19:02:43 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/24 19:02:44 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	increment_total(t_vars *vars)
{
	pthread_mutex_lock(&vars->meal_mutex);
	vars->total++;
	pthread_mutex_unlock(&vars->meal_mutex);
}

int	t_philoake_forks(t_philo *philo)
{
	if (philo->vars->simulation_end)
		return (0);
	pthread_mutex_lock(philo->right_fork);
	ft_print("has taken a fork", WHITE, philo);
	if (philo->vars->simulation_end)
		return (0);
	pthread_mutex_lock(philo->left_fork);
	ft_print("has taken a fork", WHITE, philo);
	return (1);
}

int	philo_eat(t_philo *philo, int n)
{
	if (!t_philoake_forks(philo))
		return (0);
	if (philo->vars->simulation_end)
		return (0);
	ft_print("is eating", RED, philo);
	usleep(philo->vars->time_to_eat * 1000);
	philo->last_time_eat = current_time();
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	if (n != philo->vars->number_of_meals)
		increment_total(philo->vars);
	return (1);
}

int	philo_sleep(t_philo *philo)
{
	if (philo->vars->simulation_end)
		return (0);
	ft_print("is sleeping", BLUE, philo);
	usleep(philo->vars->time_to_sleep * 1000);
	return (1);
}

void	*simulation(void *arg)
{
	t_philo	*philo;
	int		n;

	n = 0;
	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(300);
	while (!philo->vars->simulation_end)
	{
		ft_print("is thinking", GREEN, philo);
		if (!philo_eat(philo, n))
			break ;
		if (n != philo->vars->number_of_meals)
			n++;
		if (!philo_sleep(philo))
			break ;
	}
	return (NULL);
}
