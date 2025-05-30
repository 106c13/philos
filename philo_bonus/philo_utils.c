/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 19:02:43 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/30 19:51:34 by haaghaja         ###   ########.fr       */
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
		usleep(philo->vars->time_to_die);
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
		ft_print("died", DARK, philo);
		sem_wait(philo->log_sem);
		sem_post(philo->forks);
		sem_post(philo->forks);
		sem_post(philo->end_sem);
		sem_post(philo->waiter);
		return (0);
	}
	ft_print("is eating", RED, philo);
	philo->last_time_eat = current_time();
	usleep(philo->vars->time_to_eat * 1000);
	ct = current_time();
	if (ct - philo->last_time_eat > philo->vars->time_to_die)
	{
		ft_print("died", DARK, philo);
		sem_wait(philo->log_sem);
		sem_post(philo->finish_sem);
		sem_post(philo->forks);
		sem_post(philo->forks);
		sem_post(philo->end_sem);
		sem_post(philo->waiter);
		return (0);
	}
	philo->vars->total++;
	sem_post(philo->forks);
	sem_post(philo->forks);
	sem_post(philo->waiter);
	if (is_done(philo->vars))
		return (0);
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
	sem_wait(philo->finish_sem);
	if (philo->id % 2 == 0)
		usleep(500);
	while (!philo->vars->simulation_end)
	{
		ft_print("is thinking", GREEN, philo);
		if (!philo_eat(philo))
			break ;
		if (!philo_sleep(philo))
			break ;
		usleep(400);
	}
	sem_post(philo->finish_sem);
	sem_close(philo->finish_sem);
	sem_close(philo->forks);
	sem_close(philo->end_sem);
	sem_close(philo->waiter);
	sem_close(philo->log_sem);
	sem_close(philo->vars->finish_sem);
	sem_close(philo->vars->forks);
	sem_close(philo->vars->end_sem);
	sem_close(philo->vars->waiter);
	sem_close(philo->vars->log_sem);
}
