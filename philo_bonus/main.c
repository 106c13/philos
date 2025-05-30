/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 22:25:38 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/30 19:26:48 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>

void	*meal_count(void *arg)
{
	int	i;
	t_vars	*vars;
	
	usleep(2000);
	i = 0;
	vars = (t_vars *)arg;
	while (i < vars->num && !vars->simulation_end)
	{
		sem_wait(vars->finish_sem);
		i++;
		usleep(700);
	}
	sem_post(vars->log_sem);
	printf("%s Exited normally\n", DEBUG);
	sem_post(vars->end_sem);
	return (NULL);
}

void	monitor(pid_t *pids, t_vars *vars)
{
	pthread_t	mt;

	printf("%sMonitor has started\n", DEBUG);
	pthread_create(&mt, NULL, meal_count, vars);
	sem_wait(vars->end_sem);
	printf("%sMonitor stopped\n", DEBUG);
	vars->simulation_end = 1;
	pthread_join(mt, NULL);	
	for (int j = 0; j < vars->num; j++)
		kill(pids[j], SIGKILL);

	for (int i = 0; i < vars->num; i++)
	{
		int status;
		if (waitpid(pids[i], &status, 0) > 0)
		{
			printf("%sPhilosopher %d (PID %d) finished.\n", DEBUG, i + 1, pids[i]);
		}
	}
	free(pids);
	sem_close(vars->forks);	
	sem_close(vars->end_sem);	
	sem_close(vars->waiter);	
	sem_close(vars->finish_sem);	
	sem_close(vars->log_sem);
}

void	start_dining(t_vars *vars)
{
	pid_t	pid;
	t_philo	philo;
	int	i;
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * vars->num);
	if (!pids)
	{
		printf("Memory allocation failed for PIDs\n");
		return ;
	}
	vars->start_time = current_time();
	for (i = 0; i < vars->num; i++)
	{
		pid = fork();
		if (pid < 0)
		{
			printf("Error while creating a process\n");
			exit(1);
		}
		else if (pid == 0)
		{
			philo.id = i + 1;
			philo.last_time_eat = vars->start_time;
			philo.vars = vars;
			philo.forks = sem_open("/forks", 0);
			philo.end_sem = sem_open("/sim_end", 0);
			philo.waiter = sem_open("/waiter", 0);
			philo.finish_sem = sem_open("/finish", 0);
			philo.log_sem = sem_open("/log", 0);
			if (philo.forks == SEM_FAILED || philo.end_sem == SEM_FAILED)
			{
				printf("Semaphore open failed for philosopher %d\n", philo.id);
				exit(1);
			}
	//		printf("%sPhilosopher %d started. PID = %d\n", DEBUG, philo.id, getpid());
			simulation(&philo);
			free(pids);
			exit(0);
		}
		else
			pids[i] = pid;
	}
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
		sem_unlink("/forks");	
		sem_unlink("/sim_end");
		sem_unlink("/waiter");	
		sem_unlink("/finish");	
		sem_unlink("/log");	
	}
	else
		printf("Usage: ./philo n t_die t_eat t_sleep [n_eat]\n");
	return (0);
}
