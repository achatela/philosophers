/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 08:56:35 by achatela          #+#    #+#             */
/*   Updated: 2022/07/07 16:36:42 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_philos	*philos;
	pthread_t	*thread;

	if (argc < 5)
		return (printf("Not enough arguments\n"), 1);
	philos = init_philos(argv, argc);
	if (philos == NULL)
		return (2);
	thread = malloc(sizeof(pthread_t) * (ft_atoi(argv[1]) + 1));
	init_threads(philos, thread, -1, argv);
	usleep(21000);
	free(philos->start);
	ft_free(philos, argv);
	return (0);
}
