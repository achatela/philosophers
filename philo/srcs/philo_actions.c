/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 12:19:38 by achatela          #+#    #+#             */
/*   Updated: 2022/07/13 12:51:44 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philo_actions(void *param)
{
	t_philos		*philo;
	struct timeval	end;

	philo = param;
	pthread_mutex_lock(philo->m_count);
	philo->count = 0;
	pthread_mutex_unlock(philo->m_count);
	if (philo->number % 2 == 0)
		usleep(philo->time_to_eat * 200);
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
