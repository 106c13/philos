/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 19:02:30 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/31 19:44:39 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_vars(int argc, char **argv, t_vars *vars)
{
	vars->num = ft_atoi(argv[1]);
	vars->time_to_die = ft_atoi(argv[2]);
	vars->time_to_eat = ft_atoi(argv[3]);
	vars->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		vars->number_of_meals = ft_atoi(argv[5]);
	else
		vars->number_of_meals = -1;
	vars->total = 0;
	vars->simulation_end = 0;
	sem_unlink("/forks");
	sem_unlink("/sim_end");
	sem_unlink("/waiter");
	sem_unlink("/finish");
	sem_unlink("/log");
	vars->forks = sem_open("/forks", O_CREAT, 0644, vars->num);
	vars->end_sem = sem_open("/sim_end", O_CREAT, 0644, 0);
	if (vars->num == 1)
		vars->waiter = sem_open("/waiter", O_CREAT, 0644, 1);
	else
		vars->waiter = sem_open("/waiter", O_CREAT, 0644, vars->num - 1);
	vars->finish_sem = sem_open("/finish", O_CREAT, 0644, vars->num);
	vars->log_sem = sem_open("/log", O_CREAT, 0644, 1);
}
