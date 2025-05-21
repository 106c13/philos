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


void	stopSimulation(philo_t *philo)
{
	int	i;

	i = 0;
	while (philo[i].id != -1)
	{
		philo[i].simulation_end = 1;
		i++;
	}
}

void	*monitor(void *arg)
{
	philo_t	*philo;
	int	i;
	long	ct;

	philo = (philo_t *)arg;
	i = 0;
	while (1)
	{
		if (philo[i].id == -1)
			i = 0;
		//printf("[DEBUG] CHECKING %d\n", philo[i].id);
		ct = currentTime();
		if (ct - philo[i].last_time_eat > philo[i].time_to_die)
		{
			printf("[DEBUG] TIME: %ld\n", ct - philo[i].last_time_eat);
			printf("%s%ld %d died\n", DARK, passedTime(philo[i].start_time), philo[i].id);
			stopSimulation(philo);
			break ;
		}
		i++;
	}
	return (NULL);
}

int	philo_take_forks(philo_t *philo)
{	
	if (philo->simulation_end)
		return (0);
	pthread_mutex_lock(philo->right_fork);
	printf("%s%ld %d has taken a fork\n", WHITE, passedTime(philo->start_time), philo->id);
	if (philo->simulation_end)
		return (0);
	pthread_mutex_lock(philo->left_fork);
	printf("%s%ld %d has taken a fork\n", WHITE, passedTime(philo->start_time), philo->id);
	return (1);
}

int	philo_eat(philo_t *philo)
{	
	if (!philo_take_forks(philo))
		return (0);
	printf("%s%ld %d is eating\n", RED, passedTime(philo->start_time), philo->id);
	usleep(philo->time_to_eat * 1000);
	philo->last_time_eat = currentTime();
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	return (1);
}

int	philo_sleep(philo_t *philo)
{
	if (philo->simulation_end)
		return (0);
	printf("%s%ld %d is sleeping\n", BLUE, passedTime(philo->start_time), philo->id);
	usleep(philo->time_to_sleep * 1000);
	return (1);
}

void	*simulation(void *arg)
{
	philo_t	*philo = (philo_t *)arg;
	
	if (philo->id % 2 == 0)
		usleep(300);
	while (!philo->simulation_end)
	{
		printf("%s%ld %d is thinking\n", GREEN, passedTime(philo->start_time), philo->id);
		if (!philo_eat(philo))
			break ;
		if (!philo_sleep(philo))
			break ;
	}
	printf("%s[SYSTEM] %d has finished the job\n", DEBUG, philo->id);
	return (NULL);
}


philo_t	*initPhilosophers(char **argv)
{
	philo_t		*philo;
	int		id;
	int		amount;

	amount = ft_atoi(argv[1]);
	if (isNumber(argv[1]))
		philo = malloc(sizeof(philo_t) * (amount + 1));
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
	pthread_t	mthread;
	pthread_mutex_t *forks;

	if (argc == 5 || argc == 6)
	{
		num_philos = ft_atoi(argv[1]);
		philo = initPhilosophers(argv);
		threads = malloc(sizeof(pthread_t) * num_philos);
		forks = malloc(sizeof(pthread_mutex_t) * num_philos);
		if (!philo || !threads)
			return (1);
		i = 0;
		while (i < num_philos)
		{
			pthread_mutex_init(&forks[i], NULL);
			i++;
		}
		i = 0;
		start_time = currentTime();
		while (i < num_philos)
		{
			philo[i].id = i + 1;
			philo[i].start_time = start_time;
			philo[i].last_time_eat = currentTime();
			philo[i].right_fork = &forks[i];
			philo[i].left_fork = &forks[(i + 1) % num_philos];
			pthread_create(&threads[i], NULL, simulation, &philo[i]);
			i++;
		}
		philo[i].id = -1;
		pthread_create(&mthread, NULL, monitor, philo);
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
