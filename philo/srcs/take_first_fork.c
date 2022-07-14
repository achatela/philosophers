/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_first_fork.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 13:39:03 by achatela          #+#    #+#             */
/*   Updated: 2022/07/14 13:45:17 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	print_first_fork(t_philos *philo, struct timeval end)
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
		if (philo->number == 2 && philo->philo_number % 2 != 1)
			pthread_mutex_unlock(philo->right_fork);
		else
			pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	pthread_mutex_unlock(philo->m_alive);
	pthread_mutex_unlock(philo->write);
	return (0);
}

static int	take_first_fork_if(t_philos *philo, struct timeval end)
{
	pthread_mutex_unlock(philo->m_alive);
	if (philo->number == 2 && philo->philo_number % 2 != 1)
		pthread_mutex_lock(philo->right_fork);
	else
		pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(philo->m_alive);
	if (*philo->alive == 0)
	{
		if (print_first_fork(philo, end) == 1)
			return (1);
	}
	else
	{
		pthread_mutex_unlock(philo->m_alive);
		if (philo->number == 2 && philo->philo_number % 2 != 1)
			pthread_mutex_unlock(philo->right_fork);
		else
			pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	return (0);
}

void	*take_first_fork(t_philos *philo, struct timeval end)
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
		if (take_first_fork_if(philo, end) == 1)
			return (NULL);
	}
	else
	{
		pthread_mutex_unlock(philo->m_alive);
		return (NULL);
	}
	return (philo);
}
