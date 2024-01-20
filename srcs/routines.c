/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 23:30:03 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/20 19:04:36 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	eat(t_monitor *mon, t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (mon->simu->seats == 2) // 2 philos -> pick left fork first
	{
		first = &philo->right->fork;
		second = &philo->left->fork;
	}
	else if (philo->index % 2) // odd -> picks left fork first
	{
		handle_mutex_op(&mon->loop_mtx, MTX_LOCK);
		first = &philo->left->fork;
		second = &philo->right->fork;
		handle_mutex_op(&mon->loop_mtx, MTX_UNLOCK);
	}
	else // even -> picks right fork first
	{
		handle_mutex_op(&mon->loop_mtx, MTX_LOCK);
		first = &philo->right->fork;
		second = &philo->left->fork;
		handle_mutex_op(&mon->loop_mtx, MTX_UNLOCK);
	}
	handle_mutex_op(first, MTX_LOCK);
	print_status(mon, philo, "has taken a fork.");
	handle_mutex_op(second, MTX_LOCK);
	print_status(mon, philo, "has taken a fork.");
	philo->last_meal = get_time_mls();
	philo->meals++;
	print_status(mon, philo, G"is eating."RST);
	ft_usleep(mon->simu, mon->simu->time_to_eat);
	handle_mutex_op(second, MTX_UNLOCK);
	handle_mutex_op(first, MTX_UNLOCK);
}

void	*mon_routine(void *data)
{
	t_monitor	*mon;

	mon = (t_monitor *)data;
	wait_for_threads(mon->simu, mon); // monitor might not need to wait for threads
	while (!mon->simu->sim_end) // get_bool(&mon->loop_mtx, &mon->simu->sim_end)
	{
		;
	}

	mon->th_id = pthread_self();
	printf("Monit Thread ID: %lu is gone.\n", (unsigned long) mon->th_id);
	handle_thread_op(&mon->th_id, NULL, NULL, TH_DETACH);
	return NULL;
}

void	*sim_routine(void *data)
{
	t_monitor	*mon;
	t_philo	*philo;

	mon = (t_monitor *)data;
	philo = mon->philo;

	wait_for_threads(mon->simu, mon);
	philo->last_meal = get_time_mls();
	while (!get_bool(&mon->mon_mtx, &mon->simu->sim_end))
	{

		handle_mutex_op(&mon->loop_mtx, MTX_LOCK);
		philo->full = is_full(mon, philo);
		philo->dead = is_dead(mon, philo);
		handle_mutex_op(&mon->loop_mtx, MTX_UNLOCK);

		if (lone_thread(mon, philo))
			break;
		if (philo->full || philo->dead)
			break ;

		print_status(mon, philo, Y"is thinking."RST);
		//ft_usleep(mon->simu, mon->simu->seats * 100);

		eat(mon, philo); // tryna eat

		print_status(mon, philo, B"is sleeping."RST);
		ft_usleep(mon->simu, mon->simu->time_to_sleep);

	}
	//philo->th_id = pthread_self();
	//printf("Philo Thread ID: %lu is gone.\n", (unsigned long) philo->th_id);
	return NULL;
}
