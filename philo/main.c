/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 22:25:38 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/30 15:10:25 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	start_dining(t_philo *philo, t_vars *vars)
{
	pthread_t	*threads;
	int			i;

	threads = malloc(sizeof(pthread_t) * (vars->num + 1));
	if (!threads)
		return (0);
	i = 0;
	vars->start_time = current_time();
	while (i < vars->num)
	{
		philo[i].last_time_eat = vars->start_time();
		pthread_create(&threads[i], NULL, simulation, &philo[i]);
		i++;
	}
	pthread_create(&threads[vars->num], NULL, monitor, philo);
	join_threads(threads, vars->num + 1);
	free(threads);
	free(philo[0].vars->forks);
	free(philo);
	return (1);
}

int	main(int argc, char **argv)
{
	t_philo	*philo;
	t_vars	vars;

	if (argc == 5 || argc == 6)
	{
		if (!is_valid(argc, argv))
			return (error("Error!\n"));
		philo = init_philosophers(argc, argv, &vars);
		if (philo)
			start_dining(philo, philo[0].vars);
		else
			printf("Can't alocate memory\n");
	}
	else
		printf("Usage: ./philo n t_die t_eat t_sleep [n_eat]\n");
	return (0);
}
