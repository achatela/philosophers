/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 09:01:57 by achatela          #+#    #+#             */
/*   Updated: 2022/07/02 14:17:01 by achatela         ###   ########.fr       */
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

/*void	ft_lstadd_back(t_fourchette *alst, t_fourchette *new)
{
	t_fourchette	*head;

	if (!alst)
		alst = new;
	if (alst)
	{
		head = alst;
		while (head->next != NULL)
			head = head->next;
		head->next = new;
	}
}*/

/*t_fourchette	*ft_lstnew(void *content)
{
	t_fourchette	*new;

	new = malloc(sizeof(t_fourchette) * 1);
	if (!new)
		return (NULL);
	new->next = content;
	return (new);
}*/

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
		//pthread_mutex_destroy(philos->fork);
		//free(philos->fork);
		free(philos);
		philos = head;
	}
}