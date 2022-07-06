/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 09:22:37 by achatela          #+#    #+#             */
/*   Updated: 2022/07/06 15:49:50 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static t_philos	*link_forks(t_philos *philos, t_forks *forks)
{
	t_philos		*head;
	pthread_mutex_t	*write;

	write = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(write, NULL);
	head = philos;
	while (forks)
	{
		philos->write = write;
		philos->right_fork = forks->fork;
		philos->before->left_fork = forks->fork;
		forks = forks->next;
		philos = philos->next;
	}
	return (head);
}

static t_philos	*init_forks(t_philos *philos, int i)
{
	t_forks	*forks;
	t_forks	*head;
	t_forks	*begin;
	int		number;

	number = philos->before->number;
	forks = malloc(sizeof(t_forks));
	begin = forks;
	philos->free_fork = forks;
	if (!forks)
		return (NULL);
	while (++i < number)
	{
		forks->fork = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(forks->fork, NULL);
		forks->next = malloc(sizeof(t_forks));
		head = forks;
		forks = forks->next;
	}
	free(forks);
	head->next = NULL;
	philos = link_forks(philos, begin);
	return (philos);
}

static t_philos	*fill_variables2(t_philos *philos)
{
	struct timeval	*start;
	void			*alive;

	alive = malloc(sizeof(int));
	start = malloc(sizeof(struct timeval));
	if (!start || !alive)
		return (NULL);
	philos->start = start;
	philos->alive = alive;
	philos = philos->next;
	while (philos->number != 1)
	{
		philos->last_eat = 0;
		philos->alive = alive;
		philos->start = start;
		philos = philos->next;
	}
	philos = init_forks(philos, -1);
	return (philos);
}

static t_philos	*fill_variables(t_philos *philos, char **argv,
int argc, int number)
{
	int			i;
	t_philos	*head;

	i = -1;
	head = philos;
	while (++i < number)
	{
		philos->number = i + 1;
		philos->time_to_die = ft_atoi(argv[2]);
		philos->time_to_eat = ft_atoi(argv[3]);
		philos->time_to_sleep = ft_atoi(argv[4]);
		philos->must_eat = -1;
		if (argc > 5)
			philos->must_eat = ft_atoi(argv[5]);
		philos->next = malloc(sizeof(t_philos));
		philos->next->before = philos;
		philos = philos->next;
	}
	philos->before->next = head;
	head->before = philos->before;
	free(philos);
	head = fill_variables2(head);
	if (!head)
		return (NULL);
	return (head);
}

t_philos	*init_philos(char **argv, int argc)
{
	t_philos	*philos;

	if (check_args(argv, argc, -1) == 1)
		return (NULL);
	philos = malloc(sizeof(t_philos));
	if (!philos)
		return (NULL);
	philos = fill_variables(philos, argv, argc, ft_atoi(argv[1]));
	if (!philos)
		return (NULL);
	philos->start->tv_usec = 0;
	*philos->alive = 0;
	return (philos);
}
