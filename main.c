/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgamraou <sgamraou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/28 16:48:55 by sgamraou          #+#    #+#             */
/*   Updated: 2022/06/01 06:24:48 by sgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philos(t_data *data, pthread_mutex_t *myforks)
{
	int	i;

	i = 0;
	while (i < data->n_of_philos)
	{
		data->philos[i].quit = &data->quit;
		data->philos[i].print_mutex = &data->mutex;
		data->philos[i].last_eat = 0;
		data->philos[i].time_to_eat = data->time_to_eat;
		data->philos[i].time_to_sleep = data->time_to_sleep;
		data->philos[i].timu = data->timu;
		data->philos[i].myfork = myforks;
		data->philos[i].id = i;
		data->philos[i].n_of_philos = data->n_of_philos;
		data->philos[i].last_eat = data->timu;
		data->philos[i].n_eat = 0;
		i++;
	}
}

void	init_threads(char **av, int ac, t_data *data)
{
	int				i;
	pthread_mutex_t	*myforks;

	i = 0;
	data->n_of_philos = ft_atoi(av[1]);
	myforks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * \
	data->n_of_philos);
	while (i < data->n_of_philos)
	{
		pthread_mutex_init(&myforks[i], NULL);
		i++;
	}
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->number_of_turns = ft_atoi(av[5]);
	data->id = 0;
	data->timu = get_current_time();
	data->philos = malloc(sizeof(t_philos) * data->n_of_philos);
	data->threads = malloc(sizeof(pthread_t) * data->n_of_philos);
	data->quit = 0;
	pthread_mutex_init(&data->mutex, NULL);
	init_philos(data, myforks);
}

void	*philosopher(void *arg)
{
	t_philos	*philo;
	int			index;

	philo = arg;
	index = philo->id;
	if (index % 2 == 0)
		usleep (500);
	while (*(philo->quit) == 0)
	{
		pthread_mutex_lock(&philo->myfork[index]);
		print_it("\033[0;34mhas taken a fork\033[0m", philo);
		pthread_mutex_lock(&philo->myfork[(index + 1) % philo->n_of_philos]);
		print_it("\033[0;34mhas taken a fork\033[0m", philo);
		print_it("\033[0;32mis eating\033[0m", philo);
		philo->last_eat = get_current_time();
		philo->n_eat++;
		mysleep(philo->time_to_eat);
		pthread_mutex_unlock(&philo->myfork[index]);
		pthread_mutex_unlock(&philo->myfork[(index + 1) % philo->n_of_philos]);
		print_it("is sleeping", philo);
		mysleep(philo->time_to_sleep);
		print_it("\033[0;33mis thinking\033[0m", philo);
	}
	return (NULL);
}

int	kill_philophers(t_data *data, int ac)
{
	int	i;

	i = 0;
	while (i < data->n_of_philos)
	{
		if (ac == 6)
			did_everyone_eat(data);
		if (data->quit == 1)
			return (0);
		if (get_current_time() - data->philos[i].last_eat >= data->time_to_die)
		{
			pthread_mutex_lock(&data->mutex);
			data->quit = 1;
			printf("\033[0;31m%lld : philo %d died 💀\n\033[0m", \
			get_current_time() - data->philos[i].timu, data->philos[i].id + 1);
			return (0);
		}
		i = (i + 1) % data->n_of_philos;
	}
	return (1);
}

int	main(int ac, char **av)
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
	while ((data->id) < data->n_of_philos)
	{
		if (pthread_create(&data->threads[data->id], NULL, philosopher
				, &data->philos[data->id]) != 0)
		{
			printf("Error\nFailed to create thread\n");
			return (0);
		}
		data->id++;
	}
	while (kill_philophers(data, ac))
		;
	return (0);
}
