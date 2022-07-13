/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_forks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 12:32:59 by achatela          #+#    #+#             */
/*   Updated: 2022/07/13 16:38:34 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*take_forks(t_philos *philo, struct timeval end)
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
