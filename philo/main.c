/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 22:25:38 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/22 22:25:40 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor(void *arg)
{
	philo_t	*philo;
	int		i;
	long	ct;

	philo = (philo_t *)arg;
	i = 0;
	while (1)
	{
		if (philo[i].id == -1)
			i = 0;
		ct = current_time();
		if (ct - philo[i].last_time_eat > philo[i].vars->time_to_die || (philo[i].vars->total / philo[i].vars->num >= philo[i].vars->number_of_meals && philo[i].vars->number_of_meals != -1))
		{
			pthread_mutex_lock(&philo[i].vars->log_mutex);
			printf("%s%ld %d died\n", DARK, passed_time(philo[i].vars->start_time), philo[i].id);
			philo[i].vars->simulation_end = 1;
			unlock_forks(philo[0]);
			pthread_mutex_unlock(&philo[i].vars->log_mutex);
			break ;
		}
		i++;
	}
	return (NULL);
}

int	lone_philo(philo_t *philo)
{
	philo[0].vars->start_time = current_time();
	pthread_mutex_lock(philo[0].left_fork);
	printf("%s%ld %d has taken a fork\n", WHITE, passed_time(philo[0].vars->start_time), philo[0].id);
	usleep(philo[0].vars->time_to_die * 1000);
	printf("%s%ld %d died\n", DARK, passed_time(philo[0].vars->start_time), philo[0].id);
	pthread_mutex_unlock(philo[0].left_fork);
	free(philo[0].vars->forks);
	free(philo);
	return (1);
}

int	start_dining(philo_t *philo, vars_t *vars)
{
	pthread_t	*threads;
	pthread_t	mthread;
	int			i;

	if (vars->num == 1)
		return (lone_philo(philo));
	threads = malloc(sizeof(pthread_t) * vars->num);
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
	pthread_create(&mthread, NULL, monitor, philo);
	i = 0;
	while (i < vars->num)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	pthread_join(mthread, NULL);
	free(threads);
	free(philo[0].vars->forks);
	free(philo);
	return (1);
}

int	main(int argc, char **argv)
{
	philo_t	*philo;
	vars_t	vars;

	if (argc == 5 || argc == 6)
	{
		philo = init_philosophers(argc, argv, &vars);
		if (philo)
			start_dining(philo, philo[0].vars);
	}
	else
		printf("Usage: ./philo num_philos time_to_die time_to_eat time_to_sleep [optional: num_times_each_philo_must_eat]\n");
	return (0);
}
