/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 22:25:38 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/28 16:50:13 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_end(t_philo *vars)
{
	int	result;

	sem_wait(vars->end_sem);
	result = vars->vars->simulation_end;
	sem_post(vars->end_sem);
	return (result);
}

int	start_dining(t_vars *vars)
{
	pid_t		pid;
	t_philo	philo;
	int	i;
	int	status;
	pid_t pids[vars->num];

	i = 0;
	vars->start_time = current_time();
	while (i < vars->num)
	{
		pid = fork();
		if (pid < 0)
		{
			printf("Error while creating a proccess\n");
			exit(0);
		}
		else if (pid == 0)
		{
			philo.id = i + 1;
			philo.last_time_eat = current_time();
			philo.vars = vars;
			philo.forks = sem_open("/forks", 0);
			philo.end_sem = sem_open("/sem_end", 0);
			printf("%sPhilosopher %d: PID = %d\n", DEBUG, philo.id, getpid());
			simulation(&philo);
			sleep(5);
			_exit(0);
		}
		else
			pids[i] = pid;
		i++;
	}
	i = 0;
	while (i < vars->num)
	{
		if (waitpid(pids[i], &status, 0) > 0)
			printf("%sPID: %d finsied\n", DEBUG, i);
		i++;
	}
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
		if (init_philosophers(argc, argv, &vars) == 0)
			start_dining(&vars);
		else
			printf("Can't alocate memory\n");
	}
	else
		printf("Usage: ./philo n t_die t_eat t_sleep [n_eat]\n");
	return (0);
}
