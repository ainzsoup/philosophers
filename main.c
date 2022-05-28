/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgamraou <sgamraou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/28 16:48:55 by sgamraou          #+#    #+#             */
/*   Updated: 2022/05/28 21:44:11 by sgamraou         ###   ########.fr       */
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
	int  last_eat;
	pthread_mutex_t *myfork;
	int number_of_philosophers;
	int	time_to_eat;
	int	time_to_sleep;
	long long timu;
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
void	threads_init(char **av, t_data *data)
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
	i = 0;
	data->time_to_die = atoi(av[2]);
	data->time_to_eat = atoi(av[3]);
	data->time_to_sleep = atoi(av[4]);
	data->number_of_turns = atoi(av[5]);
	data->id = 0;
	data->timu = getCurrentTime();
	data->philos = malloc(sizeof(t_philos) * data->number_of_philosophers);
	data->threads = malloc(sizeof(pthread_t) * data->number_of_philosophers);
	pthread_mutex_init(&data->mutex, NULL);
	while(i < data->number_of_philosophers)
	{
		data->philos[i].print_mutex = &data->mutex;
		data->philos[i].last_eat = 0;
		data->philos[i].time_to_eat = data->time_to_eat;
		data->philos[i].time_to_sleep = data->time_to_sleep;
		data->philos[i].timu = data->timu;
		data->philos[i].myfork = myforks;
		data->philos[i].id = i;
		data->philos[i].number_of_philosophers = data->number_of_philosophers;
		i++;
	}
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
	printf("%lld : philo %d %s \n", getCurrentTime() - philo->timu, philo->id + 1, str);
	pthread_mutex_unlock(philo->print_mutex);
}

void*	philosopher(void *arg)
{
	t_philos *philo = arg;
	int index = philo->id;
	while (1)
	{
		pthread_mutex_lock(&philo->myfork[index]);
		print_it("has taken a fork", philo);
		pthread_mutex_lock(&philo->myfork[(index + 1) % philo->number_of_philosophers]);
		print_it("has taken a fork", philo);
		print_it("is eating", philo);
		philo->last_eat = getCurrentTime();
		philo->n_eat++;
		mysleep(philo->time_to_eat);
		pthread_mutex_unlock(&philo->myfork[index]);
		pthread_mutex_unlock(&philo->myfork[(index + 1) % philo->number_of_philosophers]);
		print_it("is sleeping", philo);
		mysleep(philo->time_to_sleep);
		print_it("is_thinking", philo);
	}
	return (NULL);
}


int main(int ac, char **av)
{
	t_data		*data;

	if (ac != 6)
	{
		printf("Usage: ./philo <number of philosophers> <time to die> <time to eat> <time to sleep> <time to think>\n");
		return (0);
	}
	data = malloc(sizeof(t_data));
	threads_init(av, data);
	while ((data->id) < data->number_of_philosophers)
	{
		if (pthread_create(&data->threads[data->id], NULL, philosopher, &data->philos[data->id]) != 0)
			{
				printf("Error\n");
				return (0);
			}
		usleep(20);
		data->id+=2;
	}
	usleep(50);
	data->id = 1;
	while ((data->id) < data->number_of_philosophers)
	{
		if (pthread_create(&data->threads[data->id], NULL, philosopher, &data->philos[data->id]) != 0)
			{
				printf("Error\n");
				return (0);
			}
		usleep(20);
		data->id+=2;
	}
	data->id = 0;
	while (data->id < data->number_of_philosophers)
	{
		pthread_join(data->threads[data->id], NULL);
		data->id++;
	}
	
	return (0);
}
