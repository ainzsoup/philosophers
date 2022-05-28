/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgamraou <sgamraou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 09:18:32 by sgamraou          #+#    #+#             */
/*   Updated: 2022/05/28 16:24:14 by sgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

typedef struct s_thread {
	int	mails;
	pthread_mutex_t	mutex;
}	t_thread;

void*	routine(void *arg) {	
	
	pthread_mutex_t *mutex = (pthread_mutex_t*)arg;
	t_thread *thread = (t_thread *)arg;
	for (int i = 0; i< 1000000; i++){
		pthread_mutex_lock(&thread->mutex);
		thread->mails++;
		pthread_mutex_unlock(&thread->mutex);
	}

	return NULL;	
}

int main(int ac, char **av){
	t_thread thread;
	thread.mails = 0;
	pthread_t	t1, t2; 
	pthread_t th[4];
	pthread_mutex_init(&thread.mutex, NULL);
	for (int i = 0; i < 4; i++){
		pthread_create(&th[i], NULL, routine, &thread);
		printf("thread %d started\n", i);
	}
	for (int i = 0; i < 4; i++){
		pthread_join(th[i], NULL);
		printf("thread %d finished\n", i);
	}
	pthread_mutex_destroy(&thread.mutex);
	printf("number of mails: %d\n", thread.mails);
	return (0);
}