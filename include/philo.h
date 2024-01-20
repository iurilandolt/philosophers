/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 18:21:12 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/19 23:43:59 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
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

typedef enum e_mtxcode
{
	MTX_INIT,
	MTX_DESTROY,
	MTX_LOCK,
	MTX_UNLOCK
}	t_mtxcode;

typedef enum e_thcode
{
	TH_CREATE,
	TH_JOIN,
	TH_DETACH,
}	t_thcode;

typedef struct s_fork
{
	int				index;
	pthread_mutex_t	fork;
}	t_fork;

typedef struct s_philo
{
	pthread_t	th_id;
	int			index; // must start at 1, (i + 1) // 0 is flag for is dead?
	int			meals;
	long		last_meal;
	bool		full;
	bool		dead;
	t_fork		*left;
	t_fork		*right;

}	t_philo;

typedef struct s_simu
{
	int		seats;
	int		meal_goal; // [5] or -1 (amount or flag)
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	t_philo	*philosophers;
	t_fork	*forks;
	long	sim_srt;
	bool	sim_end; // sim ends when a philo dies or all philos are full
	bool	sim_rdy;
}	t_simu;

typedef struct s_monitor
{
	t_simu	*simu;
	t_philo	*philo;
	pthread_t	th_id;
	pthread_mutex_t	mon_mtx;
	pthread_mutex_t	write_mtx;
	pthread_mutex_t	loop_mtx;
	pthread_mutex_t	dead_mtx;
}	t_monitor;

int	init_sim_values(t_simu *simu, int argc, char **argv);

void	handle_mutex_op(pthread_mutex_t *mtx, t_mtxcode op);
void	handle_thread_op(pthread_t	*thread, void *(*func)(void *), void *data, t_thcode op); // function name and arguments must be shorter xD

void	wait_for_threads(t_simu *simu, t_monitor *mon);
void	all_threads_do(t_simu *simu, t_monitor *mon, t_thcode op);

void	init_monitor(t_simu *simu, t_monitor *mon);


void	ft_error(char *str);
void	ft_usleep(t_simu *simu, long time);
bool	get_bool(pthread_mutex_t *mtx, bool *value);
void	set_bool(pthread_mutex_t *mtx, bool *var, bool value);


void	print_sim_values(t_simu *simu);

t_philo	*alloc_philos(t_simu *simu);
t_fork	*alloc_forks(t_simu *simu);

long	get_time_s(void);
long	get_time_mls(void);
long	get_time_mcs(void);

#endif
