/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 22:25:45 by haaghaja          #+#    #+#             */
/*   Updated: 2025/05/31 20:24:32 by haaghaja         ###   ########.fr       */
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
	pthread_mutex_t	*forks;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	end_mutex;
}	t_vars;

typedef struct s_philo
{
	int				id;
	long			last_time_eat;
	t_vars			*vars;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
}	t_philo;

// main.c
long	current_time(void);
long	passed_time(long start_time);

// utils.c
void	*ft_exit(void *ptr);
void	ft_print(char *str, char *color, t_philo *philo);
int		ft_atoi(char *str);

// philo_utils.c
void	increment_total(t_vars *vars);
void	*simulation(void *arg);
int		t_philoake_forks(t_philo *philo);
int		philo_eat(t_philo *philo, int n);
int		philo_sleep(t_philo *philo);

// init.c
t_philo	*init_philosophers(int argc, char **argv, t_vars *vars);
void	initForks(int num, pthread_mutex_t *forks);

// time.c
long	current_time(void);
long	passed_time(long start_time);
void	unlock_forks(t_vars *vars);

// system.h
void	join_threads(pthread_t *threads, int n);
int		ft_usleep(long ms, t_philo *philo);
int		error(char *msg);

// validate.c
int		is_valid(int argc, char **argv);

// monitor.c
void	*monitor(void *arg);
int		is_sim_end(t_vars *vars);
#endif
