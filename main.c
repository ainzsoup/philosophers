/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgamraou <sgamraou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/28 16:48:55 by sgamraou          #+#    #+#             */
/*   Updated: 2022/05/30 19:11:28 by sgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct s_philos
{
	long long  last_eat;
	pthread_mutex_t *myfork;
	int number_of_philosophers;
	int	time_to_eat;
	int	time_to_sleep;
	long long timu;
	int 		*quit;
	int n_eat;
	pthread_mutex_t *print_mutex;
	int id;
}	t_philos;

typedef struct s_data {
	int	id;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_turns;
	int	time_to_die;
	int number_of_philosophers;
	struct s_philos *philos;
	int 			quit;
	pthread_t *threads;
	pthread_mutex_t mutex;
	long long timu;
}	t_data;

long long getCurrentTime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec *1000 + tv.tv_usec / 1000);
}

void	init_philos(t_data *data, pthread_mutex_t *myforks)
{
	int	i;

	i = 0;
	while(i < data->number_of_philosophers)
	{
		data->philos[i].quit = &data->quit;
		data->philos[i].print_mutex = &data->mutex;
		data->philos[i].last_eat = 0;
		data->philos[i].time_to_eat = data->time_to_eat;
		data->philos[i].time_to_sleep = data->time_to_sleep;
		data->philos[i].timu = data->timu;
		data->philos[i].myfork = myforks;
		data->philos[i].id = i;
		data->philos[i].number_of_philosophers = data->number_of_philosophers;
		data->philos[i].last_eat = data->timu;
		data->philos[i].n_eat = 0;
		i++;
	}
}

void	init_threads(char **av, int ac, t_data *data)
{
	int i;
	pthread_mutex_t *myforks;

	i = 0;
	data->number_of_philosophers = atoi(av[1]);
	myforks = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * data->number_of_philosophers);
	while (i < data->number_of_philosophers)
	{
		pthread_mutex_init(&myforks[i], NULL);
		i++;
	}
	data->time_to_die = atoi(av[2]);
	data->time_to_eat = atoi(av[3]);
	data->time_to_sleep = atoi(av[4]);
	if (ac == 6)
		data->number_of_turns = atoi(av[5]);
	data->id = 0;
	data->timu = getCurrentTime();
	data->philos = malloc(sizeof(t_philos) * data->number_of_philosophers);
	data->threads = malloc(sizeof(pthread_t) * data->number_of_philosophers);
	data->quit = 0;
	pthread_mutex_init(&data->mutex, NULL);
	init_philos(data, myforks);
}

void mysleep(long long sleepTime)
{
	long long endTime = getCurrentTime() + sleepTime;
	while (getCurrentTime() < endTime)
	{
		usleep(50);
	}
}

void print_it(char *str, t_philos *philo)
{
	pthread_mutex_lock(philo->print_mutex);
	printf("\033[0;35m%lld \033[0m: philo %d %s \n", getCurrentTime() - philo->timu, philo->id + 1, str);
	pthread_mutex_unlock(philo->print_mutex);
}

void*	philosopher(void *arg)
{
	t_philos *philo = arg;
	int index = philo->id;
	if (index % 2 != 0)
		usleep (200);
	while (*(philo->quit) == 0)
	{
		pthread_mutex_lock(&philo->myfork[index]);
		print_it("\033[0;34mhas taken a fork\033[0m", philo);
		pthread_mutex_lock(&philo->myfork[(index + 1) % philo->number_of_philosophers]);
		print_it("\033[0;34mhas taken a fork\033[0m", philo);
		print_it("\033[0;32mis eating\033[0m", philo);
		philo->last_eat = getCurrentTime();
		philo->n_eat++;
		mysleep(philo->time_to_eat);
		pthread_mutex_unlock(&philo->myfork[index]);
		pthread_mutex_unlock(&philo->myfork[(index + 1) % philo->number_of_philosophers]);
		print_it("is sleeping", philo);
		mysleep(philo->time_to_sleep);
		print_it("\033[0;33mis thinking\033[0m", philo);
	}
	return (NULL);
}

int	everyoneAte(t_data *data)
{
	int	j;
	int	i;
	int count;

	i = 0;
	if (data->quit == 1)
		return (0);
	while (i < data->number_of_philosophers)
	{
		j = 0;
		count = 0;
		while(j < data->number_of_philosophers)
		{
			if (data->philos[j].n_eat >= data->number_of_turns)
				count++;
			j++;
		}
		if (count == data->number_of_philosophers)
		{
			pthread_mutex_lock(&data->mutex);
			data->quit = 1;
			printf("\033[0;32mEveryone ate at least %d times :)\n\033[0m", data->number_of_turns);
			return (0);
		}
		i = (i + 1) % data->number_of_philosophers;
	}
	return (1);
}

void	didEveryoneEat(t_data *data)
{
	int	count;
	int	j;
	
	count = 0;
	j = 0;
	while(j < data->number_of_philosophers)
	{
		if (data->philos[j].n_eat >= data->number_of_turns)
			count++;
		j++;
	}
	if (count == data->number_of_philosophers)
	{
		pthread_mutex_lock(&data->mutex);
		data->quit = 1;
		printf("\033[0;32mEveryone ate at least %d times :)\n\033[0m", data->number_of_turns);
	}
}

int	killPhilophers(t_data *data, int ac)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		if (ac == 6)
			didEveryoneEat(data);
		if (data->quit == 1)
			return (0);
		if (getCurrentTime() - data->philos[i].last_eat >= data->time_to_die)
		{
			pthread_mutex_lock(&data->mutex);
			data->quit = 1;
			printf("\033[0;31m%lld : philo %d died 💀\n\033[0m", getCurrentTime() - data->philos[i].timu , data->philos[i].id + 1);
			return 0;
		}
		i = (i + 1) % data->number_of_philosophers;
	}
	return (1);
}


int main(int ac, char **av)
{
	t_data		*data;

	if (ac != 6 && ac != 5)
	{
		printf("Usage: ./philosophers <number of philosophers> <time to die> <time \
			to eat> <time to sleep> <number of turns> (optional)\n");
		return (0);
	}
	data = malloc(sizeof(t_data));
	init_threads(av, ac, data);
	while ((data->id) < data->number_of_philosophers)
	{
		if (pthread_create(&data->threads[data->id], NULL, philosopher, &data->philos[data->id]) != 0)
			{
				printf("Error\nFailed to create thread\n");
				return (0);
			}
		data->id++;
	}
	while(killPhilophers(data, ac));
	return (0);
}
