/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 09:01:57 by achatela          #+#    #+#             */
/*   Updated: 2022/07/13 12:22:06 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *nptr)
{
	int	number;
	int	a;
	int	neg;

	neg = 1;
	a = 0;
	number = 0;
	while (nptr[a] == '\t' || nptr[a] == '\n' || nptr[a] == '\r'
		|| nptr[a] == '\v' || nptr[a] == '\f' || nptr[a] == ' ')
		a++;
	if (nptr[a] == '-' || nptr[a] == '+')
	{
		if (nptr[a] == '-')
			neg = -1;
		a++;
	}
	while (nptr[a] >= '0' && nptr[a] <= '9')
	{
		number = number * 10 + (nptr[a] - '0');
		a++;
	}
	return (number * neg);
}

long int	get_time(struct timeval end, struct timeval *start, t_philos *philo)
{
	long int	time;

	pthread_mutex_lock(philo->m_start);
	gettimeofday(&end, NULL);
	time = ((end.tv_sec * 1000 + end.tv_usec / 1000)
			- (start->tv_sec * 1000 + start->tv_usec / 1000));
	pthread_mutex_unlock(philo->m_start);
	return (time);
}

static void	ft_free_mutexes(t_philos *philos, pthread_mutex_t *m_global)
{
	pthread_mutex_destroy(philos->write);
	pthread_mutex_destroy(philos->m_count);
	pthread_mutex_destroy(philos->m_alive);
	pthread_mutex_destroy(philos->m_start);
	pthread_mutex_destroy(philos->m_last);
	free(philos->m_last);
	free(philos->m_count);
	free(philos->m_start);
	free(philos->m_alive);
	free(philos->write);
	free(philos->alive);
	pthread_mutex_unlock(m_global);
	pthread_mutex_destroy(m_global);
}

void	ft_free(t_philos *philos, char **argv, pthread_mutex_t *m_global)
{
	t_philos	*head;
	t_forks		*forks;
	t_forks		*head_forks;
	int			i;

	i = -1;
	pthread_mutex_lock(m_global);
	forks = philos->free_fork;
	ft_free_mutexes(philos, m_global);
	while (++i < ft_atoi(argv[1]))
	{
		head = philos->next;
		free(philos);
		philos = head;
	}
	i = -1;
	while (++i < ft_atoi(argv[1]))
	{
		head_forks = forks;
		forks = forks->next;
		pthread_mutex_destroy(head_forks->fork);
		free(head_forks->fork);
		free(head_forks);
	}
	free(m_global);
}

t_philos	*init_mutexes(t_philos *philo, int i,
	int number, t_philos *head)
{
	pthread_mutex_t	*m_alive;
	pthread_mutex_t	*m_start;
	pthread_mutex_t	*m_count;
	pthread_mutex_t	*m_last;

	m_last = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(m_last, NULL);
	m_count = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(m_count, NULL);
	m_alive = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(m_alive, NULL);
	m_start = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(m_start, NULL);
	while (++i < number)
	{
		philo->m_last = m_last;
		philo->m_count = m_count;
		philo->m_alive = m_alive;
		philo->m_start = m_start;
		philo = philo->next;
	}
	return (head);
}
