/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgamraou <sgamraou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 05:32:33 by sgamraou          #+#    #+#             */
/*   Updated: 2022/06/01 06:20:24 by sgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_philos
{
	long long		last_eat;
	pthread_mutex_t	*myfork;
	int				n_of_philos;
	int				time_to_eat;
	int				time_to_sleep;
	long long		timu;
	int				*quit;
	int				n_eat;
	pthread_mutex_t	*print_mutex;
	int				id;
}	t_philos;

typedef struct s_data {
	int				id;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_turns;
	int				time_to_die;
	int				n_of_philos;
	struct s_philos	*philos;
	int				quit;
	pthread_t		*threads;
	pthread_mutex_t	mutex;
	long long		timu;
}	t_data;

int			ft_atoi(const char *str);
long long	get_current_time(void);
void		mysleep(long long sleepTime);
void		print_it(char *str, t_philos *philo);
void		did_everyone_eat(t_data *data);

#endif