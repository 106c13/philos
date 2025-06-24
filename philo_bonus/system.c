/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 19:55:11 by haaghaja          #+#    #+#             */
/*   Updated: 2025/06/24 15:34:33 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	close_sem(t_philo *philo)
{
	sem_post(philo->finish_sem);
	sem_close(philo->finish_sem);
	sem_close(philo->sim_end_sem);
	sem_close(philo->forks);
	sem_close(philo->end_sem);
	sem_close(philo->waiter);
	sem_close(philo->log_sem);
	sem_close(philo->vars->finish_sem);
	sem_close(philo->vars->forks);
	sem_close(philo->vars->end_sem);
	sem_close(philo->vars->sim_end_sem);
	sem_close(philo->vars->waiter);
	sem_close(philo->vars->log_sem);
	sem_close(philo->vars->total_sem);
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
			break ;
		usleep(500);
	}
	return (1);
}

int	is_end(t_vars *vars)
{
	int	result;

	sem_wait(vars->sim_end_sem);
	result = vars->simulation_end;
	sem_post(vars->sim_end_sem);
	return (result);
}

void	increment_total(t_vars *vars)
{
	sem_wait(vars->total_sem);
	vars->total++;
	sem_post(vars->total_sem);
}

void	print_err(char *str)
{
	printf("%s\n", str);
	exit(1);
}
