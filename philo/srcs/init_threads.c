/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 11:20:07 by achatela          #+#    #+#             */
/*   Updated: 2022/07/06 15:21:21 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	*print_number(void *param)
{
	t_philos		*philo;
	struct timeval	end;

	philo = param;
	philo->count = 0;
	if (philo->number == 1)
		gettimeofday(philo->start, NULL);
	if (philo->number % 2 == 0)
		usleep(philo->time_to_die * 500);
	//usleep(philo->number * 1000);
	while (*philo->alive == 0 && (philo->must_eat == -1
			|| philo->count != philo->must_eat))
	{
		if (*philo->alive == 1)
			break;
		pthread_mutex_lock(philo->right_fork);
		gettimeofday(&end, NULL);
		printf("%ld %d has taken a fork\n",
			get_time(end, philo->start), philo->number);
		if (*philo->alive == 1)
			break;
		pthread_mutex_lock(philo->left_fork);
		gettimeofday(&end, NULL);
		printf("%ld %d has taken a fork\n",
			get_time(end, philo->start), philo->number);
		if (*philo->alive == 1)
			break;
		gettimeofday(&end, NULL);
		philo->last_eat = get_time(end, philo->start);
		usleep(philo->time_to_eat * 1000);
		printf("%ld %d is eating\n", get_time(end, philo->start), philo->number);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		if (*philo->alive == 1)
			break;
		gettimeofday(&end, NULL);
		printf("%ld %d is sleeping\n",
			get_time(end, philo->start), philo->number);
		usleep(philo->time_to_sleep * 1000);
		philo->count++;
	}
	return (NULL);
}

static void	*catch_death(void *philos)
{
	t_philos	*philo;
	pthread_t	*threads;
	struct timeval	end;
	int			number;
	int			i;

	philo = (t_philos *)philos;
	threads = (pthread_t *)philo->threads;
	i = -1;
	number = philo->number;
	while (1)
	{
		while (++i < number)
		{
			gettimeofday(&end, NULL);
			if (get_time(end, philo->start) - philo->last_eat >= philo->time_to_die)
			{
				pthread_mutex_lock(philo->write);
				*philo->alive = 1;
			}
			if (*philo->alive == 1)
			{
				printf("%ld %d died !\n", get_time(end, philo->start), philo->number);
				printf("Previous eating of %d was %d\n", philo->number, philo->last_eat);
				break;
			}
			if (philo->count == philo->must_eat)
				break;
			philo = philo->next;
		}		
		if (*philo->alive == 1)
			break;
		if (philo->count == philo->must_eat)
			break;
		i = -1;
	}
	i = -1;
	philo = (t_philos *)philos;
	if (*philo->alive == 1)
	{
		while (++i < philo->number)
			pthread_detach(threads[i]);
	}
	else
	{
		while (++i < philo->number)
		{
			pthread_join(threads[i], NULL);
			pthread_detach(threads[i]);
		}
	}
	return (NULL);
}

void	init_threads(t_philos *philos, pthread_t *threads, int i, char **argv)
{
	threads = malloc(sizeof(pthread_t) * (ft_atoi(argv[1]) + 1));
	while (++i < ft_atoi(argv[1]))
	{
		if (pthread_create(&threads[i], NULL,
				&print_number, philos) != 0)
			return ;
		usleep(100);
		philos = philos->next;
	}
	philos->threads = threads;
	pthread_create(&threads[i], NULL, &catch_death, philos);
	pthread_join(threads[i], NULL);
	pthread_detach(threads[i]);
	free(threads);
	pthread_mutex_unlock(philos->write);
}
