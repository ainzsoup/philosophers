/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgamraou <sgamraou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 05:39:43 by sgamraou          #+#    #+#             */
/*   Updated: 2022/06/01 06:20:35 by sgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	mysleep(long long sleepTime)
{
	long long	end_time;

	end_time = get_current_time() + sleepTime;
	while (get_current_time() < end_time)
	{
		usleep(50);
	}
}

void	print_it(char *str, t_philos *philo)
{
	pthread_mutex_lock(philo->print_mutex);
	printf("\033[0;35m%lld \033[0m: philo %d %s \n", get_current_time() - \
		philo->timu, philo->id + 1, str);
	pthread_mutex_unlock(philo->print_mutex);
}

void	did_everyone_eat(t_data *data)
{
	int	count;
	int	j;

	count = 0;
	j = 0;
	while (j < data->n_of_philos)
	{
		if (data->philos[j].n_eat >= data->number_of_turns)
			count++;
		j++;
	}
	if (count == data->n_of_philos)
	{
		pthread_mutex_lock(&data->mutex);
		data->quit = 1;
		printf("\033[0;32mEveryone ate at least %d times :)\n\033[0m"\
			, data->number_of_turns);
	}
}
