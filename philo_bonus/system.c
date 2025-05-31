/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 19:55:11 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/31 19:56:10 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	close_sem(t_philo *philo)
{
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
