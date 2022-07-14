/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 12:19:38 by achatela          #+#    #+#             */
/*   Updated: 2022/07/14 14:07:04 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	get_number(int number)
{
	if (number % 2 == 0)
		return (0);
	return (1);
}

void	*philo_while(t_philos *philo, struct timeval end)
{
	if (philo->philo_number % 2 == 1)
		usleep(1000);
	if (take_forks(philo, end) == NULL)
		return (NULL);
	usleep(10);
	if (philo_eat(philo) == NULL)
		return (NULL);
	usleep(10);
	if (philo_sleep(philo) == NULL)
		return (NULL);
	usleep(10);
	pthread_mutex_lock(philo->m_count);
	philo->count += 1;
	pthread_mutex_unlock(philo->m_count);
	return (philo);
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
		usleep(15000);
	while (philo->count != philo->must_eat)
	{
		if (philo_while(philo, end) == NULL)
			break ;
	}
	return (NULL);
}
