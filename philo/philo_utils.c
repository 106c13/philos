/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 19:02:43 by haaghaja          #+#    #+#             */
/*   Updated: 2025/06/24 13:03:34 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	increment_total(t_vars *vars)
{
	pthread_mutex_lock(&vars->meal_mutex);
	vars->total++;
	pthread_mutex_unlock(&vars->meal_mutex);
}

int	t_philo_take_forks(t_philo *philo)
{
	if (is_sim_end(philo->vars))
		return (0);
	pthread_mutex_lock(philo->right_fork);
	ft_print("has taken a fork", WHITE, philo);
	if (philo->vars->num == 1)
	{
		usleep(philo->vars->time_to_die);
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	if (is_sim_end(philo->vars))
		return (0);
	pthread_mutex_lock(philo->left_fork);
	ft_print("has taken a fork", WHITE, philo);
	return (1);
}

int	philo_eat(t_philo *philo, int n)
{
	long	ct;

	if (philo->vars->number_of_meals == 0)
		return (0);
	if (!t_philo_take_forks(philo))
		return (0);
	if (is_sim_end(philo->vars))
		return (0);
	ct = current_time();
	if (ct - philo->last_time_eat > philo->vars->time_to_die)
		return (0);
	ft_print("is eating", RED, philo);
	if (!ft_usleep(philo->vars->time_to_eat, philo))
		return (0);
	philo->last_time_eat = current_time();
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	if (n != philo->vars->number_of_meals)
		increment_total(philo->vars);
	return (1);
}

int	philo_sleep(t_philo *philo)
{
	if (is_sim_end(philo->vars))
		return (0);
	ft_print("is sleeping", BLUE, philo);
	if (!ft_usleep(philo->vars->time_to_sleep, philo))
		return (0);
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
	while (!is_sim_end(philo->vars))
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
