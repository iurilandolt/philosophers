/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 23:30:03 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/19 15:29:52 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"


void	print_status(char *status, t_monitor *mon)
{
	long	elapsed;

	elapsed = mon->simu->sim_srt;
	handle_mutex_op(&mon->write_mtx, MTX_LOCK);
	printf(W"%-6ld"RST" : %d : %s\n",  elapsed - get_time_mls(), mon->philo->index, status);
	handle_mutex_op(&mon->write_mtx, MTX_UNLOCK);
}


void	think(t_simu *simu, t_philo *philo)
{
	printf("%ld %d is thinking\n", get_time_mls(), philo->index);
}

void	sleep(t_philo *philo)
{
	printf("%ld %d is sleeping\n", get_time_mls(), philo->index);
	//ft_usleep(philo->simu->time_to_sleep);
}

void	eat(t_simu *simu, t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (simu->seats == 1)
	{
		handle_mutex_op(&philo->left->fork, MTX_LOCK);
		//ft_usleep(philo->simu->time_to_die);
		handle_mutex_op(&philo->left->fork, MTX_UNLOCK);
		return ;
	}
	else if (philo->index % 2) // odd -> picks left fork first
	{
		first = &philo->left->fork;
		second = &philo->right->fork;
	}
	else // even -> picks right fork first
	{
		first = &philo->right->fork;
		second = &philo->left->fork;
	}
	handle_mutex_op(first, MTX_LOCK);
	printf("%ld %d has taken a fork\n", get_time_mls(), philo->index);
	handle_mutex_op(second, MTX_LOCK);
	printf("%ld %d has taken a fork\n", get_time_mls(), philo->index);
	printf("%ld %d is eating\n", get_time_mls(), philo->index);
	philo->last_meal = get_time_mcs();
	philo->meals++;
	//ft_usleep(philo->simu->time_to_eat);
	handle_mutex_op(second, MTX_UNLOCK);
	handle_mutex_op(first, MTX_UNLOCK);
}
// if philo is full he should stop eating?
