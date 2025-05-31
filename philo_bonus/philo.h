/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 22:25:45 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/31 20:16:06 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <semaphore.h>
# include <stdlib.h>
#include <fcntl.h>

# define RED "\033[38;2;228;126;104m"
# define GREEN "\033[38;2;122;239;122m"
# define BLUE "\033[38;2;122;165;239m"
# define WHITE "\033[38;2;255;255;255m"
# define DARK "\033[38;2;175;0;0m"
# define DEBUG "\033[38;2;17;242;46m"

typedef struct s_vars
{
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			start_time;
	int				simulation_end;
	int				num;
	int				number_of_meals;
	int				total;
	sem_t		*end_sem;
	sem_t		*forks;
	sem_t		*waiter;
	sem_t		*finish_sem;
	sem_t		*log_sem;
}	t_vars;

typedef struct s_philo
{
	int				id;
	long			last_time_eat;
	t_vars			*vars;
	sem_t		*forks;
	sem_t		*end_sem;
	sem_t		*waiter;
	sem_t		*finish_sem;
	sem_t		*log_sem;
}	t_philo;

// utils.c
void	ft_print(char *str, char *color, t_philo *philo);
int		ft_atoi(char *str);
int		ft_exit(char *error_msg);

// philo_utils.c
void	simulation(t_philo *philo);

// init.c
void	init_vars(int argc, char **argv, t_vars *vars);

// time.c
long	current_time(void);
long	passed_time(long start_time);

// validate.c
int		is_valid(int argc, char **argv);

// system.c
void	close_sem(t_philo *philo);

#endif
