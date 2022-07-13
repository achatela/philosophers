/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 12:19:38 by achatela          #+#    #+#             */
/*   Updated: 2022/07/13 17:26:42 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	get_number(int number)
{
	if (number % 2 == 0)
		return (0);
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
		usleep(philo->time_to_eat * 200);
	while (philo->count != philo->must_eat)
	{
		usleep(333);
		if (take_forks(philo, end) == NULL)
			break ;
		usleep(333);
		if (philo_eat(philo) == NULL)
			break ;
		usleep(333);
		if (philo_sleep(philo) == NULL)
			break ;
		pthread_mutex_lock(philo->m_count);
		philo->count += 1;
		pthread_mutex_unlock(philo->m_count);
	}
	return (NULL);
}
