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
	long	ct;

	philo = (philo_t *)arg;
	i = 0;
	while (1)
	{
		if (philo[i].id == -1)
			i = 0;
		ct = currentTime();
		if (ct - philo[i].last_time_eat > philo[i].vars->time_to_die)
		{
			//printf("[DEBUG] TIME: %ld\n", ct - philo[i].last_time_eat);
			printf("%s%ld %d died\n", DARK, passedTime(philo[i].vars->start_time), philo[i].id);
			philo[i].vars->simulation_end = 1;
			break ;
		}
		i++;
	}
	return (NULL);
}

int	philo_take_forks(philo_t *philo)
{	
	if (philo->vars->simulation_end)
		return (0);
	pthread_mutex_lock(philo->right_fork);
	printf("%s%ld %d has taken a fork\n", WHITE, passedTime(philo->vars->start_time), philo->id);
	if (philo->vars->simulation_end)
		return (0);
	pthread_mutex_lock(philo->left_fork);
	printf("%s%ld %d has taken a fork\n", WHITE, passedTime(philo->vars->start_time), philo->id);
	return (1);
}

int	philo_eat(philo_t *philo)
{	
	if (!philo_take_forks(philo))
		return (0);
	if (philo->vars->simulation_end)
		return (0);
	printf("%s%ld %d is eating\n", RED, passedTime(philo->vars->start_time), philo->id);
	usleep(philo->vars->time_to_eat * 1000);
	philo->last_time_eat = currentTime();
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	return (1);
}

int	philo_sleep(philo_t *philo)
{
	if (philo->vars->simulation_end)
		return (0);
	printf("%s%ld %d is sleeping\n", BLUE, passedTime(philo->vars->start_time), philo->id);
	usleep(philo->vars->time_to_sleep * 1000);
	return (1);
}

void	*simulation(void *arg)
{
	philo_t	*philo = (philo_t *)arg;
	
	if (philo->id % 2 == 0)
		usleep(300);
	while (!philo->vars->simulation_end)
	{
		printf("%s%ld %d is thinking\n", GREEN, passedTime(philo->vars->start_time), philo->id);
		if (!philo_eat(philo))
			break ;
		if (!philo_sleep(philo))
			break ;
	}
	printf("%s[SYSTEM] %d has finished the job\n", DEBUG, philo->id);
	return (NULL);
}

void	initVars(char **argv, vars_t *vars)
{	
	vars->time_to_die = ft_atoi(argv[2]);
	vars->time_to_eat = ft_atoi(argv[3]);
	vars->time_to_sleep = ft_atoi(argv[4]);
	vars->simulation_end = 0;
}


philo_t	*initPhilosophers(char **argv, vars_t *vars)
{
	int		id;
	int		num_philos;
	philo_t		*philo;
	pthread_mutex_t *forks;

	num_philos = ft_atoi(argv[1]);
	if (isNumber(argv[1]))
		philo = malloc(sizeof(philo_t) * (num_philos + 1));
	else
		return (error("Can't allocate memory", NULL));
	forks = malloc(sizeof(pthread_mutex_t) * num_philos);
	id = 0;
	while (id < num_philos)
	{
		pthread_mutex_init(&forks[id], NULL);
		id++;
	}
	id = 0;
	vars->forks = forks;
	initVars(argv, vars);
	while (id < num_philos)
	{
		philo[id].id = id + 1;
		philo[id].vars = vars;
		philo[id].right_fork = &forks[id];
		philo[id].left_fork = &forks[(id + 1) % num_philos];
		id++;
	}
	philo[id].id = -1;
	return (philo);
}


int	lone_philo(philo_t *philo)
{
	philo[0].vars->start_time = currentTime();
	pthread_mutex_lock(philo[0].left_fork);
	printf("%s%ld %d has taken a fork\n", WHITE, passedTime(philo[0].vars->start_time), philo[0].id);
	usleep(philo[0].vars->time_to_die * 1000);
	printf("%s%ld %d died\n", DARK, passedTime(philo[0].vars->start_time), philo[0].id);
	pthread_mutex_unlock(philo[0].left_fork);
	free(philo[0].vars->forks);
	free(philo);
	return (0);
}

int	main(int argc, char **argv)
{
	int	num_philos;
	int	i;
	philo_t		*philo;
	pthread_t	*threads;
	pthread_t	mthread;
	vars_t	vars;
	

	if (argc == 5 || argc == 6)
	{
		num_philos = ft_atoi(argv[1]);
		philo = initPhilosophers(argv, &vars);
		if (num_philos == 1)
			return (lone_philo(philo));
		threads = malloc(sizeof(pthread_t) * num_philos);
		if (!philo || !threads)
			return (1);
		i = 0;
		vars.start_time = currentTime();
		while (i < num_philos)
		{
			philo[i].last_time_eat = currentTime();
			pthread_create(&threads[i], NULL, simulation, &philo[i]);
			i++;
		}
		pthread_create(&mthread, NULL, monitor, philo);
		i = 0;
		while (i < num_philos)
		{
			pthread_join(threads[i], NULL);
			i++;
		}
		free(threads);
		free(philo->vars->forks);
		free(philo);
	}
	else
	{
		printf("Usage: ./philo num_philos time_to_die time_to_eat time_to_sleep [optional: num_times_each_philo_must_eat]\n");
	}
	return (0);
}
