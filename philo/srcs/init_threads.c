/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 11:20:07 by achatela          #+#    #+#             */
/*   Updated: 2022/07/09 16:23:04 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	is_finished(t_philos *philo, int number)
{
	int	i;

	i = -1;
	while (++i < number)
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
	printf("1 returned\n");
	return (1);
}

static void	*catch_while(t_philos *philo, int number, struct timeval end, int i)
{
	pthread_mutex_lock(philo->m_alive);
	if (*philo->alive == 1)
	{
		pthread_mutex_unlock(philo->m_alive);
		return (NULL);
	}
	pthread_mutex_unlock(philo->m_alive);
	if (is_finished((t_philos *)philo, number) == 1)
		return (NULL);
	while (++i < number)
	{
		gettimeofday(&end, NULL);
		if (get_time(end, philo->start, philo)
			- philo->last_eat >= philo->time_to_die
			&& philo->count != philo->time_to_eat)
		{
			pthread_mutex_lock(philo->m_alive);
			*philo->alive = 1;
			pthread_mutex_unlock(philo->m_alive);
		}
		pthread_mutex_lock(philo->m_alive);
		if (*philo->alive == 1)
		{
			pthread_mutex_unlock(philo->m_alive);
			pthread_mutex_lock(philo->write);
			printf("%ld %d died !\n",
				get_time(end, philo->start, philo), philo->number);
			printf("Previous eating of %d was %d\n",
				philo->number, philo->last_eat);
			return (NULL);
		}
		pthread_mutex_unlock(philo->m_alive);
		philo = philo->next;
	}
	i = -1;
	return (philo);
}

static void	*catch_death(void *philos)
{
	t_philos		*philo;
	pthread_t		*threads;
	struct timeval	end;
	int				number;
	int				i;

	philo = (t_philos *)philos;
	threads = (pthread_t *)philo->threads;
	i = -1;
	number = philo->before->number;
	while (1)
	{
		if (catch_while(philos, number, end, i) == NULL)
			break ;
	}
	pthread_mutex_lock(philo->m_alive);
	while (*philo->alive == 1 && ++i < philo->number)
		pthread_detach(threads[i]);
	pthread_mutex_unlock(philo->m_alive);
	pthread_mutex_lock(philo->m_alive);
	while (*philo->alive != 1 && ++i < philo->number)
	{
		pthread_join(threads[i], NULL);
		pthread_detach(threads[i]);
	}
	pthread_mutex_unlock(philo->m_alive);
	return (NULL);
}

void	init_threads(t_philos *philos, pthread_t *threads, int i, char **argv)
{
	threads = malloc(sizeof(pthread_t) * (ft_atoi(argv[1]) + 1));
	while (++i < ft_atoi(argv[1]))
	{
		if (pthread_create(&threads[i], NULL,
				&philo_actions, philos) != 0)
			return ;
		philos = philos->next;
	}
	usleep(philos->time_to_die * 500);
	philos->threads = threads;
	pthread_create(&threads[i], NULL, &catch_death, philos);
	pthread_join(threads[i], NULL);
	pthread_detach(threads[i]);
	free(threads);
	pthread_mutex_unlock(philos->write);
}
