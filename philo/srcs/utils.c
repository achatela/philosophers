/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 09:01:57 by achatela          #+#    #+#             */
/*   Updated: 2022/07/06 14:30:04 by achatela         ###   ########.fr       */
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

long int	get_time(struct timeval end, struct timeval *start)
{
	long int	time;

	time = ((end.tv_sec * 1000 + end.tv_usec / 1000)
			- (start->tv_sec * 1000 + start->tv_usec / 1000));
	return (time);
}

void	ft_free(t_philos *philos, char **argv)
{
	t_philos	*head;
	t_forks		*free_forks;
	t_forks		*head_forks;
	int			i;

	free_forks = philos->free_fork;
	//i = ft_atoi(argv[1]);
	i = -1;
	free(philos->alive);
	pthread_mutex_unlock(philos->write);
	pthread_mutex_destroy(philos->write);
	free(philos->write);
	while (++i < ft_atoi(argv[1]))
	{
		head = philos->next;
		free(philos);
		philos = head;
	}
	while (free_forks != NULL)
	{
		head_forks = free_forks;
		free_forks = free_forks->next;
		pthread_mutex_destroy(head_forks->fork);
		free(head_forks->fork);
		free(head_forks);
	}
}
