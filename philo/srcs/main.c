/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 08:56:35 by achatela          #+#    #+#             */
/*   Updated: 2022/07/14 14:57:34 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	check_validity(char **argv, int i, int j, int argc)
{
	while (++i < argc)
	{
		while (argv[i][j] != '\0')
		{
			if (j == 0 && argv[i][j] == '\0')
			{
				printf("Empty argument\n");
				return (1);
			}
			if ((argv[i][j] > 57 || argv[i][j] < 48))
			{
				printf("Non numeric argument\n");
				return (1);
			}
			j++;
		}
		if (j == 0)
		{
			printf("Empty argument\n");
			return (1);
		}
		j = 0;
	}
	return (0);
}

static void	*thread_one_philo(void *param)
{
	struct timeval	end;
	t_philos		*philo;

	philo = (t_philos *)param;
	gettimeofday(philo->start, NULL);
	pthread_mutex_lock(philo->left_fork);
	gettimeofday(&end, NULL);
	philo->last_eat = 0;
	printf("%ld %d has taken a fork\n",
		get_time(end, philo->start, philo), philo->number);
	while (1)
	{
		if (get_time(end, philo->start, philo)
			- philo->last_eat > philo->time_to_die)
		{
			pthread_mutex_unlock(philo->left_fork);
			printf("%ld %d died !\n",
				get_time(end, philo->start, philo), philo->number);
			break ;
		}
	}
	return (NULL);
}

static void	free_one_philo(t_philos *philos)
{
	pthread_mutex_destroy(philos->write);
	pthread_mutex_destroy(philos->m_count);
	pthread_mutex_destroy(philos->m_alive);
	pthread_mutex_destroy(philos->m_start);
	pthread_mutex_destroy(philos->m_last);
	free(philos->start);
	free(philos->m_last);
	free(philos->m_count);
	free(philos->m_start);
	free(philos->m_alive);
	free(philos->write);
	free(philos->alive);
	free(philos->left_fork);
	free(philos->free_fork);
	free(philos);
}

static void	only_one_philo(t_philos *philo,
	pthread_t *thread)
{
	pthread_create(&thread[0], NULL, &thread_one_philo, philo);
	pthread_join(thread[0], NULL);
	pthread_detach(thread[0]);
	free_one_philo(philo);
	free(thread);
}

int	main(int argc, char **argv)
{
	t_philos		*philos;
	pthread_t		*thread;
	pthread_mutex_t	*m_global;

	if (argc < 5)
		return (printf("Not enough arguments\n"), 1);
	if (argc >= 7)
		return (printf("Too much arguments\n"), 1);
	if (check_validity(argv, 0, 0, argc) == 1)
		return (2);
	philos = init_philos(argv, argc);
	if (philos == NULL)
		return (3);
	thread = malloc(sizeof(pthread_t) * (ft_atoi(argv[1]) + 1));
	if (ft_atoi(argv[1]) == 1)
		return (only_one_philo(philos, thread), 4);
	m_global = init_threads(philos, thread, -1, argv);
	free(philos->start);
	usleep(50000);
	free(thread);
	ft_free(philos, argv, m_global);
	return (0);
}
