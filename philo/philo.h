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
	int		simulation_end;
	int		num;
	int		number_of_meals;
	int		total;
	pthread_mutex_t	*forks;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	meal_mutex;
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
long	current_time();
long	passed_time(long start_time);
void	*simulation();


// utils.c
unsigned int	ft_atoi(char *str);
int	isNumber(char *str);
void	*ft_exit(void *ptr);
void	ft_print(char *str, char *color, philo_t *philo);


// philo_utils.c
void	increment_total(vars_t *vars);
void	*simulation(void *arg);
int	philo_take_forks(philo_t *philo);
int	philo_eat(philo_t *philo, int n);
int	philo_sleep(philo_t *philo);

// init.c
pthread_mutex_t	*initForks(int num);
void	init_vars(int argc, char **argv, vars_t *vars);
philo_t	*init_philosophers(int	argc, char **argv, vars_t *vars);

// time.c
long	current_time(void);
long	passed_time(long start_time);
void	unlock_forks(philo_t philo);

#endif
