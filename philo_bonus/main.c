/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 22:25:38 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/26 19:34:05 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


int	start_dining(t_vars *vars)
{
	pid_t		pid;
	int			i;

	i = 0;
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
			printf("%sPhilosopher %d: PID = %d\n", DEBUG, i+1, getpid());
			sleep(5);
			_exit(0);
		}
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
