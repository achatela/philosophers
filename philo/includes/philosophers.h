/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 08:58:02 by achatela          #+#    #+#             */
/*   Updated: 2022/07/02 16:48:20 by achatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <time.h>
# include <sys/time.h>

typedef struct s_forks	t_forks;
struct s_forks
{
	pthread_mutex_t	*fork;
	t_forks			*next;
};

typedef struct s_philos	t_philos;
struct s_philos
{
	int				*alive;
	int				number;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	int				last_eat;
	pthread_t		*threads;
	struct timeval	*start;
	pthread_mutex_t	*write;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_forks			*free_fork;
	t_philos		*next;
	t_philos		*before;
};

int			ft_atoi(const char *nptr);
//void	ft_lstadd_back(t_fourchette *alst, t_fourchette *new);
//t_fourchette	*ft_lstnew(void *content);
long int	get_time(struct timeval end, struct timeval *start);
int			check_args(char **argv, int argc, int i);
void		ft_free(t_philos *philos, char **argv);
t_philos	*init_philos(char **argv, int argc);
void		init_threads(t_philos *philos, pthread_t *threads,
				int i, char **argv);
void		free_pthreads(pthread_t *pthreads);

#endif
