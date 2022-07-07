/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 12:19:38 by achatela          #+#    #+#             */
/*   Updated: 2022/07/07 12:19:56 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	*take_forks(t_philos *philo, struct timeval end)
{
	if (*philo->alive == 1)
		return (NULL);
	pthread_mutex_lock(philo->left_fork);
	if (*philo->alive == 0)
	{
		pthread_mutex_lock(philo->write);
		gettimeofday(&end, NULL);
		if (*philo->alive == 0)
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
		if (*philo->alive == 0)
			printf("%ld %d has taken a fork\n",
				get_time(end, philo->start), philo->number);
		pthread_mutex_unlock(philo->write);
	}
	return (philo);
}

static void	*philo_eat(t_philos *philo)
{
	struct timeval	end;

	if (*philo->alive == 1)
		return (NULL);
	if (*philo->alive == 0)
	{
		pthread_mutex_lock(philo->write);
		gettimeofday(&end, NULL);
		if (*philo->alive == 0)
			printf("%ld %d is eating\n",
				get_time(end, philo->start), philo->number);
		pthread_mutex_unlock(philo->write);
	}
	philo->last_eat = get_time(end, philo->start);
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (philo);
}

static void	*philo_sleep(t_philos *philo)
{
	struct timeval	end;

	if (*philo->alive == 1)
		return (NULL);
	if (*philo->alive == 0)
	{
		pthread_mutex_lock(philo->write);
		gettimeofday(&end, NULL);
		if (*philo->alive == 0)
			printf("%ld %d is sleeping\n",
				get_time(end, philo->start), philo->number);
		pthread_mutex_unlock(philo->write);
	}
	usleep(philo->time_to_sleep * 1000);
	return (philo);
}

void	*philo_actions(void *param)
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
	{
		if (take_forks(philo, end) == NULL)
			break ;
		usleep(1000);
		if (philo_eat(philo) == NULL)
			break ;
		usleep(1000);
		if (philo_sleep(philo) == NULL)
			break ;
		philo->count++;
	}
	return (NULL);
}