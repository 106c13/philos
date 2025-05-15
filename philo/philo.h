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


typedef struct philo_s
{
	pthread_t	tid;
	int		id;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	int		n;
} philo_t;



// main.c
long	currentTime();
long	passedTime(long start_time);
void	simulation();


// utils.c
unsigned int	ft_atoi(char *str);
int	isNumber(char *str);
void	*error(char *str, void *ptr);

#endif
