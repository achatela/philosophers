/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_sleep.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 11:40:59 by achatela          #+#    #+#             */
/*   Updated: 2022/07/14 14:07:03 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	philo_sleep2(t_philos *philo, struct timeval end)
{
	pthread_mutex_unlock(philo->m_alive);
	pthread_mutex_lock(philo->m_alive);
	if (*philo->alive == 0)
	{
		pthread_mutex_unlock(philo->m_alive);
		pthread_mutex_lock(philo->write);
		pthread_mutex_lock(philo->m_alive);
		if (*philo->alive == 0)
			printf("%ld %d is sleeping\n",
				get_time(end, philo->start, philo), philo->number);
		if (*philo->alive == 0)
			printf("%ld %d is thinking\n",
				get_time(end, philo->start, philo), philo->number);
		pthread_mutex_unlock(philo->m_alive);
		pthread_mutex_unlock(philo->write);
	}
	else
		pthread_mutex_unlock(philo->m_alive);
}

void	*philo_sleep(t_philos *philo)
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
		philo_sleep2(philo, end);
	else
		pthread_mutex_unlock(philo->m_alive);
	usleep(philo->time_to_sleep * 1000);
	return (philo);
}
