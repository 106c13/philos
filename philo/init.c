/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 19:02:30 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/26 18:45:46 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t	*init_forks(int num)
{
	pthread_mutex_t	*forks;

	forks = malloc(sizeof(pthread_mutex_t) * num);
	if (!forks)
		return (NULL);
	while (num > 0)
	{
		num--;
		pthread_mutex_init(&forks[num], NULL);
	}
	return (forks);
}

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
	vars->forks = init_forks(vars->num);
	pthread_mutex_init(&vars->log_mutex, NULL);
	pthread_mutex_init(&vars->meal_mutex, NULL);
	pthread_mutex_init(&vars->end_mutex, NULL);
}

t_philo	*init_philosophers(int argc, char **argv, t_vars *vars)
{
	int		id;
	t_philo	*philo;

	init_vars(argc, argv, vars);
	if (!vars->forks)
		return (NULL);
	philo = malloc(sizeof(t_philo) * (vars->num + 1));
	if (!philo)
		return (ft_exit(vars->forks));
	id = 0;
	while (id < vars->num)
	{
		philo[id].id = id + 1;
		philo[id].vars = vars;
		philo[id].right_fork = &(vars->forks[id]);
		philo[id].left_fork = &(vars->forks[(id + 1) % vars->num]);
		id++;
	}
	philo[id].id = -1;
	return (philo);
}
