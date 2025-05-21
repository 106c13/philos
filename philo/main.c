#include "philo.h"


long	currentTime()
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L) + (tv.tv_usec / 1000);
}

long	passedTime(long start_time)
{
	return (currentTime() - start_time);
}


void	*monitor(void *arg)
{
	philo_t	*philo;
	int	i;

	philo = (philo_t *)arg;
	while (1)
	{
	}
	return (NULL);
}

void	*simulation(void *arg)
{
	philo_t	*philo = (philo_t *)arg;
	long	start_time = philo->start_time;
	
	if (philo->id % 2 == 0)
		usleep(10);
	while (1)
	{
		printf("%s%ld %d is thinking\n", GREEN, passedTime(start_time), philo->id);
		pthread_mutex_lock(philo->right_fork);
		printf("%s%ld %d has taken a fork\n", WHITE, passedTime(start_time), philo->id);
		pthread_mutex_lock(philo->left_fork);
		printf("%s%ld %d has taken a fork\n", WHITE, passedTime(start_time), philo->id);
		printf("%s%ld %d is eating\n", RED, passedTime(start_time), philo->id);
		usleep(philo->time_to_eat * 1000);
		philo->last_time_eat = currentTime();
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		printf("%s%ld %d is sleeping\n", BLUE, passedTime(start_time), philo->id);
		usleep(philo->time_to_sleep * 1000);
	}
	return (NULL);
}


philo_t	*initPhilosophers(char **argv)
{
	philo_t		*philo;
	int		id;
	int		amount;

	amount = ft_atoi(argv[1]);
	if (isNumber(argv[1]))
		philo = malloc(sizeof(philo_t) * amount);
	else
		return (error("Can't allocate memory", NULL));
	id = 0;
	while (id < amount)
	{
		philo[id].time_to_die = ft_atoi(argv[2]);
		philo[id].time_to_eat = ft_atoi(argv[3]);
		philo[id].time_to_sleep = ft_atoi(argv[4]);
		id++;
	}
	return (philo);
}

int	main(int argc, char **argv)
{
	int	num_philos;
	int	i;
	long	start_time;
	philo_t		*philo;
	pthread_t	*threads;
	pthread_mutex_t *forks;

	if (argc == 5 || argc == 6)
	{
		num_philos = ft_atoi(argv[1]);
		philo = initPhilosophers(argv);
		threads = malloc(sizeof(pthread_t) * num_philos);
		forks = malloc(sizeof(pthread_mutex_t) * num_philos);
		if (!philo || !threads)
			return (1);
		start_time = currentTime();
		i = 0;
		while (i < num_philos)
		{
			pthread_mutex_init(&forks[i], NULL);
			i++;
		}
		i = 0;
		while (i < num_philos)
		{
			philo[i].id = i + 1;
			philo[i].start_time = start_time;
			philo[i].last_time_eat = -1;
			philo[i].right_fork = &forks[i];
			philo[i].left_fork = &forks[(i + 1) % num_philos];
			pthread_create(&threads[i], NULL, simulation, &philo[i]);
			i++;
		}
		i = 0;
		while (i < num_philos)
		{
			pthread_join(threads[i], NULL);
			i++;
		}
		free(threads);
		free(philo);
	}
	else
	{
		printf("Usage: ./philo num_philos time_to_die time_to_eat time_to_sleep [optional: num_times_each_philo_must_eat]\n");
	}
	return (0);
}
