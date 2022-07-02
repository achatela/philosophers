/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 09:01:57 by achatela          #+#    #+#             */
/*   Updated: 2022/07/02 18:18:10 by achatela         ###   ########.fr       */
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
	int			i;

	i = ft_atoi(argv[1]);
	free(philos->alive);
	while (i-- > 0)
	{
		head = philos->next;
		free(philos->left_fork);
		free(philos);
		philos = head;
	}
}
