/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:38:17 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/26 18:44:34 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_dead(t_philo philo)
{
	long	ct;

	ct = current_time();
	if (ct - philo.last_time_eat > philo.vars->time_to_die)
		return (1);
	return (0);
}

int	is_end(t_vars *vars)
{
	int	total;

	if (vars->number_of_meals == -1)
		return (0);
	pthread_mutex_lock(&vars->meal_mutex);
	total = vars->total;
	pthread_mutex_unlock(&vars->meal_mutex);
	if (total / vars->num >= vars->number_of_meals)
		return (1);
	return (0);
}

int	is_sim_end(t_vars *vars)
{
	int	result;

	pthread_mutex_lock(&vars->end_mutex);
	result = vars->simulation_end;
	pthread_mutex_unlock(&vars->end_mutex);
	return (result);
}

void	stop_simulation(t_vars *vars)
{
	pthread_mutex_lock(&vars->log_mutex);
	pthread_mutex_lock(&vars->end_mutex);
	vars->simulation_end = 1;
	pthread_mutex_unlock(&vars->end_mutex);
	unlock_forks(vars);
	pthread_mutex_unlock(&vars->log_mutex);
}

void	*monitor(void *arg)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)arg;
	i = 0;
	while (1)
	{
		if (philo[i].id == -1)
			i = 0;
		if (is_dead(philo[i]))
		{
			ft_print("died", DARK, &philo[i]);
			stop_simulation(philo[i].vars);
			break ;
		}
		if (is_end(philo[i].vars))
		{
			stop_simulation(philo[i].vars);
			break ;
		}
		i++;
	}
	return (NULL);
}
