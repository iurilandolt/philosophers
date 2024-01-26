/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 18:21:12 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/24 15:35:15 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <limits.h>

# define R		"\033[0;31m"
# define G		"\033[0;32m"
# define B		"\033[0;34m"
# define W		"\033[0;37m"
# define C		"\033[0;36m"
# define M		"\033[0;35m"
# define Y		"\033[0;33m"
# define RST	"\033[0m"

typedef struct s_fork
{
	int				index;
	pthread_mutex_t	mtx;
}	t_fork;

typedef struct s_sim	t_sim;

typedef struct s_philo
{
	pthread_t			th_id;
	int					index;
	int					meals;
	int					limit;
	int					lspan;
	long				last_meal;
	bool				full;
	bool				dead;
	t_fork				*left;
	t_fork				*right;
	t_sim				*sim;
	pthread_mutex_t		mtx;
}	t_philo;

typedef struct s_sim
{
	t_philo			*philosophers;
	t_fork			*forks;
	int				seats;
	int				goal;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long			start;
	bool			ended;
	pthread_mutex_t	mtx;
}	t_sim;

int		parse(t_sim *sim, int argc, char **argv);
void	ft_error(char *str);
void	startup(t_sim *sim);
void	cleanup(t_sim *sim);
void	threads_create(t_sim *sim);
t_philo	*alloc_philos(t_sim *sim);
t_fork	*alloc_forks(t_sim *sim);
void	*philosopher(void *data);
bool	is_dead(t_philo *philo);
bool	is_full(t_philo *philo);
bool	lone_thread(t_philo *philo);
bool	get_bool(pthread_mutex_t *mtx, bool *value);
void	print_status(t_philo *philo, char *status);
void	pickup_first_fork(t_philo *philo);
void	pickup_second_fork(t_philo *philo);
void	putdown_first_fork(t_philo *philo);
void	putdown_second_fork(t_philo *philo);
void	ft_usleep(long usecs);
long	get_time(void);

void	print_sim_values(t_sim *sim);

#endif
