/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 08:56:35 by achatela          #+#    #+#             */
/*   Updated: 2022/07/02 14:53:56 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_philos	*philos;

	if (argc < 5)
		return (printf("Not enough arguments\n"), 1);
	philos = init_philos(argv, argc);
	if (philos == NULL)
		return (2);
	init_threads(philos, NULL, -1, argv);
	free(philos->start);
	sleep(1);
	ft_free(philos, argv);
	return (0);
}