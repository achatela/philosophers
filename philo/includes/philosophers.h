/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achatela <achatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 08:58:02 by achatela          #+#    #+#             */
/*   Updated: 2022/07/13 12:34:50 by achatela         ###   ########.fr       */
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
	int				count;
	pthread_t		*threads;
	struct timeval	*start;
	pthread_mutex_t	*write;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*m_alive;
	pthread_mutex_t	*m_count;
	pthread_mutex_t	*m_start;
	pthread_mutex_t	*m_last;
	t_forks			*free_fork;
	t_philos		*next;
	t_philos		*before;
};

int					ft_atoi(const char *nptr);
long int			get_time(struct timeval end,
						struct timeval *start, t_philos *philo);
int					check_args(char **argv, int argc, int i);
void				ft_free(t_philos *philos, char **argv,
						pthread_mutex_t *m_global);
t_philos			*init_philos(char **argv, int argc);
pthread_mutex_t		*init_threads(t_philos *philos, pthread_t *threads,
						int i, char **argv);
void				free_pthreads(pthread_t *pthreads);
t_philos			*init_mutexes(t_philos *philo, int i,
						int number, t_philos *head);

/**
 *	Philos_Actions
 **/
void				*philo_actions(void *param);
void				*philo_sleep(t_philos *philo);
void				*philo_eat(t_philos *philo);
void				*take_forks(t_philos *philo, struct timeval end);
/**
 *	Philos_Actions
 **/

#endif
