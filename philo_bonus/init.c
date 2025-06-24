/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 19:02:30 by haaghaja          #+#    #+#             */
/*   Updated: 2025/06/24 15:14:08 by haaghaja         ###   ########.fr       */
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
	unlink_sem();
	vars->forks = sem_open("/forks", O_CREAT, 0644, vars->num);
	vars->end_sem = sem_open("/sim_end", O_CREAT, 0644, 0);
	vars->sim_end_sem = sem_open("/sim_end_sem", O_CREAT, 0644, 1);
	vars->total_sem = sem_open("/total_sem", O_CREAT, 0644, 1);
	if (vars->num == 1)
		vars->waiter = sem_open("/waiter", O_CREAT, 0644, 1);
	else
		vars->waiter = sem_open("/waiter", O_CREAT, 0644, vars->num - 1);
	vars->finish_sem = sem_open("/finish", O_CREAT, 0644, vars->num);
	vars->log_sem = sem_open("/log", O_CREAT, 0644, 1);
}

void	setup_philo(t_philo *philo, int id)
{
	philo->id = id;
	philo->last_time_eat = philo->vars->start_time;
	philo->forks = sem_open("/forks", 0);
	philo->end_sem = sem_open("/sim_end", 0);
	philo->sim_end_sem = sem_open("/sim_end_sem", 1);
	philo->waiter = sem_open("/waiter", 0);
	philo->finish_sem = sem_open("/finish", 0);
	philo->log_sem = sem_open("/log", 0);
	if (philo->forks == SEM_FAILED || philo->end_sem == SEM_FAILED)
		print_err("Failed to open semaphore for philosopher");
}

int	init_philos(t_vars *vars, pid_t *pids)
{
	int		i;
	pid_t	pid;
	t_philo	philo;

	i = 0;
	while (i < vars->num)
	{
		pid = fork();
		if (pid < 0)
			print_err("Error while creating a process");
		else if (pid == 0)
		{
			philo.vars = vars;
			setup_philo(&philo, i + 1);
			simulation(&philo);
			free(pids);
			exit(0);
		}
		else
			pids[i] = pid;
		i++;
	}
	return (1);
}

void	unlink_sem(void)
{
	sem_unlink("/forks");
	sem_unlink("/sim_end");
	sem_unlink("/sim_end_sem");
	sem_unlink("/waiter");
	sem_unlink("/finish");
	sem_unlink("/log");
	sem_unlink("/total_sem");
}
