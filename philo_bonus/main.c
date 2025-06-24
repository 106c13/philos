/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 22:25:38 by haaghaja          #+#    #+#             */
/*   Updated: 2025/06/24 15:42:08 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>

void	*meal_count(void *arg)
{
	int		i;
	t_vars	*vars;

	usleep(2000);
	i = 0;
	vars = (t_vars *)arg;
	while (i < vars->num && !is_end(vars))
	{
		sem_wait(vars->finish_sem);
		i++;
		usleep(700);
	}
	sem_post(vars->end_sem);
	return (NULL);
}

int	philo_sleep(t_philo *philo)
{
	ft_print("is sleeping", BLUE, philo);
	ft_usleep(philo->vars->time_to_sleep, philo);
	return (1);
}

void	monitor(pid_t *pids, t_vars *vars)
{
	pthread_t	mt;
	int			i;
	int			status;

	pthread_create(&mt, NULL, meal_count, vars);
	sem_wait(vars->end_sem);
	vars->simulation_end = 1;
	pthread_join(mt, NULL);
	i = 0;
	while (i < vars->num)
	{
		kill(pids[i], SIGKILL);
		if (waitpid(pids[i], &status, 0) < 0)
			printf("%s%d (PID %d) failed to finish.", DEBUG, i + 1, pids[i]);
		i++;
	}
	sem_post(vars->log_sem);
	free(pids);
	sem_close(vars->forks);
	sem_close(vars->end_sem);
	sem_close(vars->sim_end_sem);
	sem_close(vars->waiter);
	sem_close(vars->finish_sem);
	sem_close(vars->log_sem);
	sem_close(vars->total_sem);
}

void	start_dining(t_vars *vars)
{
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * vars->num);
	if (!pids)
	{
		printf("Memory allocation failed for PIDs\n");
		return ;
	}
	vars->start_time = current_time();
	if (!init_philos(vars, pids))
		return ;
	monitor(pids, vars);
}

int	main(int argc, char **argv)
{
	t_vars	vars;

	if (argc == 5 || argc == 6)
	{
		if (!is_valid(argc, argv))
			return (ft_exit("Error!"));
		init_vars(argc, argv, &vars);
		start_dining(&vars);
		unlink_sem();
	}
	else
		printf("Usage: ./philo n t_die t_eat t_sleep [n_eat]\n");
	return (0);
}
