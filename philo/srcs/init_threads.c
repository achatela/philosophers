/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 11:20:07 by achatela          #+#    #+#             */
/*   Updated: 2022/07/12 17:38:59 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
		return (pthread_mutex_lock(philo->write), NULL);
	while (++i < number)
	{
		gettimeofday(&end, NULL);
		pthread_mutex_lock(philo->m_count);
		pthread_mutex_lock(philo->m_last);
		if (get_time(end, philo->start, philo)
			- philo->last_eat >= philo->time_to_die
			&& philo->count != philo->must_eat)
		{
			pthread_mutex_unlock(philo->m_last);
			pthread_mutex_unlock(philo->m_count);
			pthread_mutex_lock(philo->m_alive);
			*philo->alive = 1;
			pthread_mutex_unlock(philo->m_alive);
		}
		else
		{
			pthread_mutex_unlock(philo->m_last);
			pthread_mutex_unlock(philo->m_count);
		}
		pthread_mutex_lock(philo->m_alive);
		if (*philo->alive == 1)
		{
			pthread_mutex_unlock(philo->m_alive);
			pthread_mutex_lock(philo->write);
			gettimeofday(&end, NULL);
			printf("%ld %d died !\n",
				get_time(end, philo->start, philo), philo->number);
			printf("Previous eating of %d was %d\n",
				philo->number, philo->last_eat);
			return (NULL);
		}
		pthread_mutex_unlock(philo->m_alive);
		philo = philo->next;
	}
	return (philo);
}

static void	*catch_death(void *philos)
{
	t_philos		*philo;
	pthread_t		*threads;
	struct timeval	end;
	int				number;
	int				i;
	int				j;

	j = 0;
	philo = (t_philos *)philos;
	threads = (pthread_t *)philo->threads;
	i = -1;
	number = philo->before->number;
	while (1)
	{
		if (catch_while(philos, number, end, i) == NULL)
		{
			j = 1;
			break ;
		}
	}
	pthread_mutex_lock(philo->m_alive);
	i = -1;
	if (*philo->alive == 1)
	{
		pthread_mutex_unlock(philo->m_alive);
		while (++i < number)
		{
			pthread_join(threads[i], NULL);
			pthread_detach(threads[i]);
		}
	}
	else
	{
		pthread_mutex_unlock(philo->m_alive);
		while (++i < number)
		{
			pthread_join(threads[i], NULL);
			pthread_detach(threads[i]);
		}
	}
	if (j == 1)
		pthread_mutex_unlock(philo->write);
	return (NULL);
}

pthread_mutex_t	*init_threads(t_philos *philos, pthread_t *threads, int i, char **argv)
{
	pthread_mutex_t	*m_global;

	m_global = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(m_global, NULL);
	pthread_mutex_lock(m_global);
	threads = malloc(sizeof(pthread_t) * (ft_atoi(argv[1]) + 1));
	/*while (++i < 5)
	{
		printf("number %d left fork = %p right fork = %p\n", philos->number, philos->left_fork, philos->right_fork);
		philos = philos->next;
	}
	i = -1;*/
	while (++i < ft_atoi(argv[1]))
	{
		if (pthread_create(&threads[i], NULL,
				&philo_actions, philos) != 0)
		{
			pthread_mutex_unlock(m_global);
			return (m_global);
		}
		philos = philos->next;
	}
	usleep(philos->time_to_die * 500);
	philos->threads = threads;
	pthread_create(&threads[i], NULL, &catch_death, philos);
	pthread_join(threads[i], NULL);
	pthread_detach(threads[i]);
	free(threads);
	pthread_mutex_unlock(m_global);
	return (m_global);
}
