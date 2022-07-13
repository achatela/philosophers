/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 12:19:38 by achatela          #+#    #+#             */
/*   Updated: 2022/07/13 12:22:18 by achatela         ###   ########.fr       */
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
	pthread_mutex_lock(philo->m_alive);
	if (*philo->alive == 0)
	{
		pthread_mutex_unlock(philo->m_alive);
		if (philo->number == 2)
			pthread_mutex_lock(philo->right_fork);
		else
			pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(philo->m_alive);
		if (*philo->alive == 0)
		{
			pthread_mutex_unlock(philo->m_alive);
			pthread_mutex_lock(philo->write);
			pthread_mutex_lock(philo->m_alive);
			if (*philo->alive == 0)
				printf("%ld %d has taken a fork\n",
					get_time(end, philo->start, philo), philo->number);
			else
			{
				pthread_mutex_unlock(philo->m_alive);
				pthread_mutex_unlock(philo->write);
				if (philo->number == 2)
					pthread_mutex_unlock(philo->right_fork);
				else
					pthread_mutex_unlock(philo->left_fork);
				return (NULL);
			}
			pthread_mutex_unlock(philo->m_alive);
			pthread_mutex_unlock(philo->write);
		}
		else
		{
			pthread_mutex_unlock(philo->m_alive);
			if (philo->number == 2)
				pthread_mutex_unlock(philo->right_fork);
			else
				pthread_mutex_unlock(philo->left_fork);
			return (NULL);
		}
	}
	else
	{
		pthread_mutex_unlock(philo->m_alive);
		return (NULL);
	}
	pthread_mutex_lock(philo->m_alive);
	if (*philo->alive == 0)
	{
		pthread_mutex_unlock(philo->m_alive);
		if (philo->number == 2)
			pthread_mutex_lock(philo->left_fork);
		else
			pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(philo->m_alive);
		if (*philo->alive == 0)
		{
			pthread_mutex_unlock(philo->m_alive);
			pthread_mutex_lock(philo->write);
			pthread_mutex_lock(philo->m_alive);
			if (*philo->alive == 0)
				printf("%ld %d has taken a fork\n",
					get_time(end, philo->start, philo), philo->number);
			else
			{
				pthread_mutex_unlock(philo->m_alive);
				pthread_mutex_unlock(philo->write);
				pthread_mutex_unlock(philo->left_fork);
				pthread_mutex_unlock(philo->right_fork);
				return (NULL);
			}
			pthread_mutex_unlock(philo->m_alive);
			pthread_mutex_unlock(philo->write);
		}
		else
		{
			pthread_mutex_unlock(philo->m_alive);
			pthread_mutex_unlock(philo->right_fork);
			pthread_mutex_unlock(philo->left_fork);
			return (NULL);
		}
	}
	else
	{
		pthread_mutex_unlock(philo->m_alive);
		if (philo->number == 2)
			pthread_mutex_unlock(philo->right_fork);
		else
			pthread_mutex_unlock(philo->left_fork);
		return (NULL);
	}
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
	if (philo->number % 2 == 0)
		usleep(philo->time_to_eat * 500);
	while (philo->count != philo->must_eat)
	{
		if (take_forks(philo, end) == NULL)
			break ;
		if (philo_eat(philo) == NULL)
			break ;
		if (philo_sleep(philo) == NULL)
			break ;
		pthread_mutex_lock(philo->m_count);
		philo->count += 1;
		pthread_mutex_unlock(philo->m_count);
	}
	return (NULL);
}
