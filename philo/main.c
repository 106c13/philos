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

long	currentTime()
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L) + (tv.tv_usec / 1000);
}

long	passedTime(long start_time)
{
	return (currentTime() - start_time);
}

void	unlock_forks(philo_t philo)
{
	int	i;

	i = 0;
	while (i < philo.vars->num)
	{
		pthread_mutex_unlock(&philo.vars->forks[i]);
		i++;
	}
}

void	*monitor(void *arg)
{
	philo_t	*philo;
	int	i;
	long	ct;

	philo = (philo_t *)arg;
	i = 0;
	while (1)
	{
		if (philo[i].id == -1)
			i = 0;
		ct = currentTime();
		if (ct - philo[i].last_time_eat > philo[i].vars->time_to_die)
		{
			pthread_mutex_lock(&philo[i].vars->log_mutex);
			printf("%s%ld %d died\n", DARK, passedTime(philo[i].vars->start_time), philo[i].id);
			philo[i].vars->simulation_end = 1;
			unlock_forks(philo[0]);
			pthread_mutex_unlock(&philo[i].vars->log_mutex);
			break ;
		}
		i++;
	}
	return (NULL);
}

int	philo_take_forks(philo_t *philo)
{	
	if (philo->vars->simulation_end)
		return (0);
	pthread_mutex_lock(philo->right_fork);
	ft_print("has taken a fork", WHITE, philo);
	if (philo->vars->simulation_end)
		return (0);
	pthread_mutex_lock(philo->left_fork);
	ft_print("has taken a fork", WHITE, philo);
	return (1);
}

int	philo_eat(philo_t *philo)
{	
	if (!philo_take_forks(philo))
		return (0);
	if (philo->vars->simulation_end)
		return (0);
	ft_print("is eating", RED, philo);
	usleep(philo->vars->time_to_eat * 1000);
	philo->last_time_eat = currentTime();
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	return (1);
}

int	philo_sleep(philo_t *philo)
{
	if (philo->vars->simulation_end)
		return (0);
	ft_print("is sleeping", BLUE, philo);
	usleep(philo->vars->time_to_sleep * 1000);
	return (1);
}

void	*simulation(void *arg)
{
	philo_t	*philo = (philo_t *)arg;
	
	if (philo->id % 2 == 0)
		usleep(300);
	while (!philo->vars->simulation_end)
	{
		ft_print("is thinking", GREEN, philo);
		if (!philo_eat(philo))
			break ;
		if (!philo_sleep(philo))
			break ;
	}
	//printf("%s[SYSTEM] %d has finished the job\n", DEBUG, philo->id);
	return (NULL);
}

pthread_mutex_t	*initForks(int	num)
{
	pthread_mutex_t *forks;

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

void	initVars(char **argv, vars_t *vars)
{	
	vars->num = ft_atoi(argv[1]);
	vars->time_to_die = ft_atoi(argv[2]);
	vars->time_to_eat = ft_atoi(argv[3]);
	vars->time_to_sleep = ft_atoi(argv[4]);
	vars->simulation_end = 0;
	vars->forks = initForks(vars->num);
	pthread_mutex_init(&vars->log_mutex, NULL);
}


philo_t	*initPhilosophers(char **argv, vars_t *vars)
{
	int		id;
	philo_t		*philo;

	initVars(argv, vars);
	if (!vars->forks)
		return (NULL);
	philo = malloc(sizeof(philo_t) * (vars->num + 1));
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


int	lone_philo(philo_t *philo)
{
	philo[0].vars->start_time = currentTime();
	pthread_mutex_lock(philo[0].left_fork);
	printf("%s%ld %d has taken a fork\n", WHITE, passedTime(philo[0].vars->start_time), philo[0].id);
	usleep(philo[0].vars->time_to_die * 1000);
	printf("%s%ld %d died\n", DARK, passedTime(philo[0].vars->start_time), philo[0].id);
	pthread_mutex_unlock(philo[0].left_fork);
	free(philo[0].vars->forks);
	free(philo);
	return (1);
}

int	startDining(philo_t *philo, vars_t *vars)
{
	pthread_t	*threads;
	pthread_t	mthread;
	int		i;
	
	if (vars->num == 1)
		return (lone_philo(philo));
	threads = malloc(sizeof(pthread_t) * vars->num);
	if (!philo || !threads)
		return (0);
	i = 0;
	vars->start_time = currentTime();
	while (i < vars->num)
	{
		philo[i].last_time_eat = currentTime();
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
		philo = initPhilosophers(argv, &vars);
		if (philo)
			startDining(philo, philo[0].vars);
	}
	else
		printf("Usage: ./philo num_philos time_to_die time_to_eat time_to_sleep [optional: num_times_each_philo_must_eat]\n");
	return (0);
}
