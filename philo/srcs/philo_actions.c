/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 12:19:38 by achatela          #+#    #+#             */
/*   Updated: 2022/07/14 12:24:20 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	get_number(int number)
{
	if (number % 2 == 0)
		return (0);
	return (1);
}

static int	is_finished(t_philos *philo, int number)
{
	int	i;

	i = -1;
	while (++i != number)
	{
		pthread_mutex_lock(philo->m_count);
		if (philo->count != philo->must_eat)
		{
			pthread_mutex_unlock(philo->m_count);
			return (0);
		}
		pthread_mutex_unlock(philo->m_count);
		philo = philo->next;
	}
	return (1);
}

void	*philo_actions(void *param)
{
	t_philos		*philo;
	struct timeval	end;
	int				modulo;

	philo = param;
	pthread_mutex_lock(philo->m_count);
	philo->count = 0;
	pthread_mutex_unlock(philo->m_count);
	modulo = get_number(philo->philo_number);
	if (philo->number % 2 == modulo)
		//usleep(philo->time_to_eat * 1000);
		usleep(15000);
	while (philo->count != philo->must_eat)
	{
		if (take_forks(philo, end) == NULL)
			break ;
		usleep(10);
		if (philo_eat(philo) == NULL)
			break ;
		usleep(10);
		if (philo_sleep(philo) == NULL)
			break ;
		usleep(10);
		pthread_mutex_lock(philo->m_count);
		philo->count += 1;
		pthread_mutex_unlock(philo->m_count);
		if (philo->must_eat != -1 && is_finished(philo, philo->philo_number) == 1)
			break;
	}
	return (NULL);
}
