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

void	simulation(philo_t *philo, int size)
{
	long	ct;
	int	id;

	ct = currentTime();
	id = 0;
	while (1)
	{
		printf("%s%ld %d has taken a fork\n", WHITE, passedTime(ct), id);
		printf("%s%ld %d is eating\n", RED, passedTime(ct), id);
		usleep(philo[id].time_to_eat * 1000);
		printf("%s%ld %d is sleeping\n", BLUE, passedTime(ct), id);
		usleep(philo[id].time_to_sleep * 1000);
		printf("%s%ld %d is thinking\n", GREEN, passedTime(ct), id);
		id++;
		if (id == size)
			id = 0;
	}	
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
	philo_t	*philo; // array of philosophers
	
	if (argc == 5 || argc == 6)
	{
		philo = initPhilosophers(argv);
		if (!philo)
			return (0);
		simulation(philo, ft_atoi(argv[1]));
	}



	printf("Simulation stated...\n");
	//pthread_create(&philo, NULL, simulation, NULL);
	//pthread_join(philo, NULL);
	free(philo);
	printf("Simulation stoped...\n");
	return (0);
}
