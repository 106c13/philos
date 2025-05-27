#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>


sem_t	semaphore;

void	*foo(int *a)
{
	int	i;
	printf("%p\n",a);
	i = 0;
	while (i < 1000)
	{
		sem_wait(&semaphore);
		(*a)++;
		i++;
		sem_post(&semaphore);
	}
	return (NULL);
}

int	main()
{
	int	*a;
	pid_t	pid;
	sem_init(&semaphore, 1, 1);
	*a = 0;
	pid = fork();
	if (pid < 0)
	{
		perror("Failed to create process: ");
		return (1);
	}
	printf("Created PID: %d\n", pid);
	foo(a);
	printf("%d\n", *a);
	return (0);
}
