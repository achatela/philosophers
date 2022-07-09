/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 12:19:38 by achatela          #+#    #+#             */
/*   Updated: 2022/07/09 19:15:32 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	*take_forks(t_philos *philo, struct timeval end)
{
	pthread_mutex_lock(philo->m_alive);
	if (*philo->alive == 1)
	{
		pthread_mutex_unlock(philo->m_alive);
		return (NULL);
	}
	pthread_mutex_unlock(philo->m_alive);
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(philo->m_alive);
	if (*philo->alive == 0)
	{
		pthread_mutex_unlock(philo->m_alive);
		pthread_mutex_lock(philo->write);
		gettimeofday(&end, NULL);
		pthread_mutex_lock(philo->m_alive);
		if (*philo->alive == 0)
		{
			pthread_mutex_unlock(philo->m_alive);
			printf("%ld %d has taken a fork\n",
				get_time(end, philo->start, philo), philo->number);
			pthread_mutex_unlock(philo->write);
		}
		else
			pthread_mutex_unlock(philo->m_alive);
	}
	pthread_mutex_lock(philo->m_alive);
	if (*philo->alive == 1)
	{
		pthread_mutex_unlock(philo->m_alive);
		pthread_mutex_unlock(philo->left_fork);
		return (NULL);
	}
	pthread_mutex_unlock(philo->m_alive);
	pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(philo->m_alive);
	if (*philo->alive == 0)
	{
		pthread_mutex_unlock(philo->m_alive);
		pthread_mutex_lock(philo->write);
		gettimeofday(&end, NULL);
		pthread_mutex_lock(philo->m_alive);
		if (*philo->alive == 0)
		{
			pthread_mutex_unlock(philo->m_alive);
			printf("%ld %d has taken a fork\n",
				get_time(end, philo->start, philo), philo->number);
			pthread_mutex_unlock(philo->write);
		}
		else
			pthread_mutex_unlock(philo->m_alive);
	}
	return (philo);
}

static void	*philo_eat(t_philos *philo)
{
	struct timeval	end;

	pthread_mutex_lock(philo->m_alive);
	if (*philo->alive == 1)
	{
		pthread_mutex_unlock(philo->m_alive);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		return (NULL);
	}
	pthread_mutex_unlock(philo->m_alive);
	pthread_mutex_lock(philo->m_alive);
	if (*philo->alive == 0)
	{
		pthread_mutex_unlock(philo->m_alive);
		pthread_mutex_lock(philo->write);
		gettimeofday(&end, NULL);
		pthread_mutex_lock(philo->m_alive);
		if (*philo->alive == 0)
		{
			pthread_mutex_unlock(philo->m_alive);
			printf("%ld %d is eating\n",
				get_time(end, philo->start, philo), philo->number);
			pthread_mutex_unlock(philo->write);
		}
		else
			pthread_mutex_unlock(philo->m_alive);
	}
	philo->last_eat = get_time(end, philo->start, philo);
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_lock(philo->m_count);
	philo->count += 1;
	pthread_mutex_unlock(philo->m_count);
	return (philo);
}

static void	*philo_sleep(t_philos *philo)
{
	struct timeval	end;

	pthread_mutex_lock(philo->m_alive);
	if (*philo->alive == 1)
	{
		pthread_mutex_unlock(philo->m_alive);
		return (NULL);
	}
	pthread_mutex_unlock(philo->m_alive);
	pthread_mutex_lock(philo->m_alive);
	if (*philo->alive == 0)
	{
		pthread_mutex_unlock(philo->m_alive);
		pthread_mutex_lock(philo->write);
		gettimeofday(&end, NULL);
		pthread_mutex_lock(philo->m_alive);
		if (*philo->alive == 0)
		{
			pthread_mutex_unlock(philo->m_alive);
			printf("%ld %d is sleeping\n",
				get_time(end, philo->start, philo), philo->number);
			pthread_mutex_unlock(philo->write);
		}
		else
			pthread_mutex_unlock(philo->m_alive);
	}
	usleep(philo->time_to_sleep * 1000);
	return (philo);
}

void	*philo_actions(void *param)
{
	t_philos		*philo;
	struct timeval	end;

	philo = param;
	pthread_mutex_lock(philo->m_count);
	philo->count = 0;
	pthread_mutex_unlock(philo->m_count);
	if (philo->number == 1)
	{
		pthread_mutex_lock(philo->m_start);
		gettimeofday(philo->start, NULL);
		pthread_mutex_unlock(philo->m_start);
	}
	if (philo->number != 1)
		usleep(1000);
	if (philo->number % 2 == 0)
		usleep(philo->time_to_eat * 100);
	while (philo->count != philo->must_eat)
	{
		if (take_forks(philo, end) == NULL)
			break ;
		usleep(1000);
		if (philo_eat(philo) == NULL)
			break ;
		usleep(1000);
		if (philo_sleep(philo) == NULL)
			break ;
	}
	return (NULL);
}
