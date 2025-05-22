/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 22:25:45 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/22 22:25:46 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>

# define RED "\033[38;2;228;126;104m"
# define GREEN "\033[38;2;122;239;122m"
# define BLUE "\033[38;2;122;165;239m"
# define WHITE "\033[38;2;255;255;255m"
# define DARK "\033[38;2;175;0;0m"
# define DEBUG "\033[38;2;17;242;46m"

typedef struct vars_s
{

	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	start_time;
	long	f;
	int		simulation_end;
	int		num;
	pthread_mutex_t	*forks;
	pthread_mutex_t	log_mutex;
}	vars_t;

typedef struct philo_s
{
	int				id;
	long			last_time_eat;
	vars_t	*vars;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
}	philo_t;

// main.c
long	currentTime();
long	passedTime(long start_time);
void	*simulation();


// utils.c
unsigned int	ft_atoi(char *str);
int	isNumber(char *str);
void	*ft_exit(void *ptr);
void	ft_print(char *str, char *color, philo_t *philo);

#endif
