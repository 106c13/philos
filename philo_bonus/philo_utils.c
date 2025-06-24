/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 19:02:43 by haaghaja          #+#    #+#             */
/*   Updated: 2025/06/24 15:39:19 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	t_philo_take_forks(t_philo *philo)
{
	sem_wait(philo->waiter);
	sem_wait(philo->forks);
	ft_print("has taken a fork", WHITE, philo);
	if (philo->vars->num == 1)
	{
		ft_usleep(philo->vars->time_to_die, philo);
		ft_print("died", DARK, philo);
		sem_post(philo->forks);
		sem_post(philo->end_sem);
		sem_post(philo->waiter);
		return (0);
	}
	sem_wait(philo->forks);
	ft_print("has taken a fork", WHITE, philo);
	return (1);
}

int	is_done(t_vars *vars)
{
	if (vars->number_of_meals == -1)
		return (0);
	if (vars->total >= vars->number_of_meals)
		return (1);
	return (0);
}

void	philo_exit(t_philo *philo)
{
	sem_wait(philo->log_sem);
	sem_post(philo->forks);
	sem_post(philo->forks);
	sem_post(philo->end_sem);
	sem_post(philo->waiter);
}

int	philo_eat(t_philo *philo)
{
	if (philo->vars->number_of_meals == 0)
		return (0);
	if (!t_philo_take_forks(philo))
		return (0);
	if (is_time_passed(philo))
		return (0);
	ft_print("is eating", RED, philo);
	ft_usleep(philo->vars->time_to_eat, philo);
	if (is_time_passed(philo))
		return (0);
	philo->last_time_eat = current_time();
	increment_total(philo->vars);
	sem_post(philo->forks);
	sem_post(philo->forks);
	sem_post(philo->waiter);
	if (is_done(philo->vars))
		return (0);
	return (1);
}

void	simulation(t_philo *philo)
{
	sem_wait(philo->finish_sem);
	if (philo->id % 2 == 0)
		usleep(500);
	while (!is_end(philo->vars))
	{
		ft_print("is thinking", GREEN, philo);
		if (!philo_eat(philo))
			break ;
		if (!philo_sleep(philo))
			break ;
		usleep(400);
	}
	close_sem(philo);
}
