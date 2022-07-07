/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 11:20:07 by achatela          #+#    #+#             */
/*   Updated: 2022/07/07 11:55:57 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	*take_forks(t_philos *philo)
{
	struct timeval	end;

	if (*philo->alive == 1)
		return (NULL);
	pthread_mutex_lock(philo->left_fork);
	if (*philo->alive == 0)
	{
			pthread_mutex_lock(philo->write);
		gettimeofday(&end, NULL);
		printf("%ld %d has taken a fork\n",
			get_time(end, philo->start), philo->number);
		pthread_mutex_unlock(philo->write);
	}
	if (*philo->alive == 1)
		return (NULL);
	pthread_mutex_lock(philo->right_fork);
	if (*philo->alive == 0)
	{
		pthread_mutex_lock(philo->write);
		gettimeofday(&end, NULL);
		printf("%ld %d has taken a fork\n",
			get_time(end, philo->start), philo->number);
		pthread_mutex_unlock(philo->write);
	}
	return (philo);
}

static void	*philo_actions(void *param)
{
	t_philos		*philo;
	struct timeval	end;

	philo = param;
	philo->count = 0;
	if (philo->number == 1)
		gettimeofday(philo->start, NULL);
	if (philo->number != 1)
		usleep(1000);
	if (philo->number % 2 == 0)
		usleep(philo->time_to_eat * 100);
	while (*philo->alive == 0 && (philo->must_eat == -1
			|| philo->count != philo->must_eat))
	{/*
		if (*philo->alive == 1)
			break ;
		pthread_mutex_lock(philo->left_fork);
		if (*philo->alive == 0)
		{
			pthread_mutex_lock(philo->write);
			gettimeofday(&end, NULL);
			printf("%ld %d has taken a fork\n",
				get_time(end, philo->start), philo->number);
			pthread_mutex_unlock(philo->write);
		}
		if (*philo->alive == 1)
			break ;
		pthread_mutex_lock(philo->right_fork);
		if (*philo->alive == 0)
		{
			pthread_mutex_lock(philo->write);
			gettimeofday(&end, NULL);
			printf("%ld %d has taken a fork\n",
				get_time(end, philo->start), philo->number);
			pthread_mutex_unlock(philo->write);
		}*/
		if (take_forks(philo) == NULL)
			break;
		usleep(1000);
		if (*philo->alive == 1)
			break ;
		if (*philo->alive == 0)
		{
			pthread_mutex_lock(philo->write);
			gettimeofday(&end, NULL);
			printf("%ld %d is eating\n", get_time(end, philo->start), philo->number);
			pthread_mutex_unlock(philo->write);
		}
		philo->last_eat = get_time(end, philo->start);
		usleep(philo->time_to_eat * 1000);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		usleep(1000);
		if (*philo->alive == 1)
			break ;
		if (*philo->alive == 0)
		{
			pthread_mutex_lock(philo->write);
			gettimeofday(&end, NULL);
			printf("%ld %d is sleeping\n",
				get_time(end, philo->start), philo->number);
			pthread_mutex_unlock(philo->write);
		}
		usleep(philo->time_to_sleep * 1000);
		philo->count++;
	}
	return (NULL);
}

static int	is_finished(t_philos *philo, int number)
{
	int	i;

	i = -1;
	while (++i < number)
	{
		if (philo->count != philo->must_eat)
			return (0);
		else
			philo = philo->next;
	}
	return (1);
}

static void	*catch_death(void *philos)
{
	t_philos		*philo;
	pthread_t		*threads;
	struct timeval	end;
	int				number;
	int				i;

	philo = (t_philos *)philos;
	threads = (pthread_t *)philo->threads;
	i = -1;
	number = philo->number;
	while (1)
	{
		if (*philo->alive == 1 || is_finished((t_philos *)philos, number) == 1)
			break ;
		while (++i < number)
		{
			gettimeofday(&end, NULL);
			if (get_time(end, philo->start)
				- philo->last_eat >= philo->time_to_die
				&& philo->count != philo->time_to_eat)
				*philo->alive = 1;
			if (*philo->alive == 1)
			{
		/*		if (pthread_mutex_lock(philo->write) == EBUSY)
				{
					pthread_mutex_unlock(philo->write);*/
					pthread_mutex_lock(philo->write);
		//		}
				printf("%ld %d died !\n",
					get_time(end, philo->start), philo->number);
				printf("Previous eating of %d was %d\n",
					philo->number, philo->last_eat);
				break ;
			}
			philo = philo->next;
		}
		i = -1;
	}
	while (*philo->alive == 1 && ++i < philo->number)
		pthread_detach(threads[i]);
	while (*philo->alive != 1 && ++i < philo->number)
	{
		pthread_join(threads[i], NULL);
		pthread_detach(threads[i]);
	}
	pthread_mutex_unlock(philo->write);
	return (NULL);
}

void	init_threads(t_philos *philos, pthread_t *threads, int i, char **argv)
{
	threads = malloc(sizeof(pthread_t) * (ft_atoi(argv[1]) + 1));
	while (++i < ft_atoi(argv[1]))
	{
		if (pthread_create(&threads[i], NULL,
				&philo_actions, philos) != 0)
			return ;
		philos = philos->next;
	}
	usleep(philos->time_to_die * 500);
	philos->threads = threads;
	pthread_create(&threads[i], NULL, &catch_death, philos);
	pthread_join(threads[i], NULL);
	pthread_detach(threads[i]);
	free(threads);
}
