/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 22:25:38 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/24 18:59:38 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor(void *arg)
{
	t_philo	*philo;
	int		i;
	long	ct;

	philo = (t_philo *)arg;
	i = 0;
	while (1)
	{
		if (philo[i].id == -1)
			i = 0;
		ct = current_time();
		if (ct - philo[i].last_time_eat > philo[i].vars->time_to_die || (philo[i].vars->total / philo[i].vars->num >= philo[i].vars->number_of_meals && philo[i].vars->number_of_meals != -1))
		{
			ft_print("died", DARK, &philo[i]);
			pthread_mutex_lock(&philo[i].vars->log_mutex);
			philo[i].vars->simulation_end = 1;
			unlock_forks(philo[0]);
			pthread_mutex_unlock(&philo[i].vars->log_mutex);
			break ;
		}
		i++;
	}
	return (NULL);
}

int	lone_philo(t_philo *philo)
{
	philo[0].vars->start_time = current_time();
	pthread_mutex_lock(philo[0].left_fork);
	ft_print("has taken a fork", WHITE, &philo[0]);
	usleep(philo[0].vars->time_to_die * 1000);
	ft_print("died", DARK, &philo[0]);
	pthread_mutex_unlock(philo[0].left_fork);
	free(philo[0].vars->forks);
	free(philo);
	return (1);
}

int	start_dining(t_philo *philo, t_vars *vars)
{
	pthread_t	*threads;
	int			i;

	if (vars->num == 1)
		return (lone_philo(philo));
	threads = malloc(sizeof(pthread_t) * (vars->num + 1));
	if (!philo || !threads)
		return (0);
	i = 0;
	vars->start_time = current_time();
	while (i < vars->num)
	{
		philo[i].last_time_eat = current_time();
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
		philo = init_philosophers(argc, argv, &vars);
		if (philo)
			start_dining(philo, philo[0].vars);
	}
	else
		printf("Usage: ./philo n t_die t_eat t_sleep [n_eat]\n");
	return (0);
}
