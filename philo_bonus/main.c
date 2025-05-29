/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 22:25:38 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/29 17:36:07 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	start_dining(t_vars *vars)
{
	pid_t		pid;
	t_philo		philo;
	int			i;
	pid_t		*pids;

	vars->start_time = current_time();
	pids = malloc(sizeof(pid_t) * vars->num);
	if (!pids)
	{
		printf("Memory allocation failed for PIDs\n");
		return (0);
	}
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
			philo.last_time_eat = current_time();
			philo.vars = vars;
			philo.forks = sem_open("/forks", 0);
			philo.end_sem = sem_open("/sim_end", 0);

			if (philo.forks == SEM_FAILED || philo.end_sem == SEM_FAILED)
			{
				printf("Semaphore open failed for philosopher %d\n", philo.id);
				exit(1);
			}

			printf("%sPhilosopher %d started. PID = %d\n", DEBUG, philo.id, getpid());
			simulation(&philo);  // This should handle eat/sleep/think
			_exit(0);
		}
		else
			pids[i] = pid;
	}
	printf("%sMonitor has started\n", DEBUG);
	sem_wait(vars->end_sem);
	printf("%sMonitor stopped\n", DEBUG);
	
	for (int j = 0; j < vars->num; j++)
		kill(pids[j], SIGKILL);

	for (i = 0; i < vars->num; i++)
	{
		int status;
		if (waitpid(pids[i], &status, 0) > 0)
		{
			printf("Philosopher %d (PID %d) finished.\n", i + 1, pids[i]);
		}
	}
	free(pids);
	sem_close(vars->forks);	
	sem_close(vars->end_sem);	
	return (1);
}

int	ft_exit(char *error_msg)
{
	printf("%s\n", error_msg);
	return (1);
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
	}
	else
		printf("Usage: ./philo n t_die t_eat t_sleep [n_eat]\n");
	return (0);
}
