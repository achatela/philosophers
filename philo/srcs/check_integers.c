/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_integers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 09:10:14 by achatela          #+#    #+#             */
/*   Updated: 2022/07/02 09:48:39 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static int	ft_compare(char **tab, int i, char *str)
{
	int	j;
	int	k;

	j = 0;
	k = 10;
	if (tab[i][0] == '-')
		k = 11;
	while (j < k)
	{
		if (tab[i][j] > str[j])
			return (-1);
		j++;
	}
	return (0);
}

int	check_args(char **argv, int argc, int i)
{
	if (argc > 6)
		argc = 6;
	while (++i < argc)
	{
		if ((ft_strlen(argv[i]) > 10) || (ft_strlen(argv[i]) == 10
				&& ft_compare(argv, i, "2147483647") == -1))
			return ((printf("Integer overflow\n"), 1));
		if (ft_atoi(argv[i]) < 0)
			return (printf("Invalid format\n"), 1);
	}
	if (ft_atoi(argv[2]) <= (ft_atoi(argv[3]) + ft_atoi(argv[4])))
		return (printf("Time to die is lower than sleeping + eating time\n"), 1);
	return (0);
}
