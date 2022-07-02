/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 11:20:07 by achatela          #+#    #+#             */
/*   Updated: 2022/07/02 18:19:22 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	kill_philo(t_philos *philo, struct timeval end)
{
	if (*philo->alive == 1)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->write);
		return ;
	}
	*philo->alive = 1;
	gettimeofday(&end, NULL);
	printf("%ld %d died !\n", get_time(end, philo->start), philo->number);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->write);
}

static void	*print_number(void *param)
{
	t_philos		*philo;
	int				count;
	struct timeval	end;

	count = 0;
	philo = param;
	if (philo->number == 1)
		gettimeofday(philo->start, NULL);
	if ((philo->number % 2) == 0)
		usleep(1000);
	usleep(philo->number * 10);
	while (*philo->alive == 0 && (philo->must_eat == -1
			|| count != philo->must_eat))
	{
		gettimeofday(&end, NULL);
		pthread_mutex_lock(philo->write);
		pthread_mutex_lock(philo->right_fork);
		printf("%ld %d has taken a fork\n",
			get_time(end, philo->start), philo->number);
		pthread_mutex_lock(philo->left_fork);
		printf("%ld %d has taken a fork\n",
			get_time(end, philo->start), philo->number);
		if (get_time(end, philo->start) - philo->last_eat >= philo->time_to_die)
			return (kill_philo(philo, end), NULL);
		gettimeofday(&end, NULL);
		philo->last_eat = get_time(end, philo->start);
		printf("%ld %d is eating\n", get_time(end, philo->start), philo->number);
		pthread_mutex_unlock(philo->write);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		if (*philo->alive == 1)
			return (NULL);
		usleep(philo->time_to_eat * 1000);
		gettimeofday(&end, NULL);
		if (*philo->alive == 1)
			return (NULL);
		pthread_mutex_lock(philo->write);
		printf("%ld %d is sleeping\n",
			get_time(end, philo->start), philo->number);
		pthread_mutex_unlock(philo->write);
		usleep(philo->time_to_sleep * 1000);
		count++;
	}
	return (NULL);
}

static void	*catch_death(void *philos)
{
	t_philos	*philo;
	pthread_t	*threads;
	int			i;

	philo = (t_philos *)philos;
	threads = (pthread_t *)philo->threads;
	i = -1;
	while (++i < philo->number)
	{
		pthread_join(threads[i], NULL);
		pthread_detach(threads[i]);
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
		philos = philos->next;
	}
	philos->threads = threads;
	pthread_create(&threads[i], NULL, &catch_death, philos);
	pthread_join(threads[i], NULL);
	pthread_detach(threads[i]);
	free(threads);
}
