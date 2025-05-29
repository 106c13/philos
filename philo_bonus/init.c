/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 19:02:30 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/29 17:36:21 by haaghaja         ###   ########.fr       */
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
	vars->forks = sem_open("/forks", O_CREAT, 0644, vars->num);
	vars->end_sem = sem_open("/sim_end", O_CREAT, 0644, 0);
}

int	init_philosophers(int argc, char **argv, t_vars *vars)
{
	init_vars(argc, argv, vars);
	if (!vars)
		return (1);
	return (0);
}
