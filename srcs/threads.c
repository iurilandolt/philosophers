/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:42:15 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/18 19:24:24 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/*
void philo_cycle(t_philo *philo)
{
	philo think - usleep N ms?
	pick up left fork - lock mutex?
	pick up right fork - lock mutex?
	philo eat - usleep N ms?
	put down left fork - unlock mutex?
	put down right fork - unlock mutex?
}
*/


//spinlock / busy waiting


void	wait_for_threads(t_simu *simu)
{
	while (!simu->sim_rdy)
		;
}

void	*sim_routine(void *data)
{
	t_monitor	*mon;

	mon = (t_monitor *)data;
	wait_for_threads(mon->simu);
	return NULL;
}

// also need extra thread for monitoring philos
//handle_thread_op(mon->th_id, mon_routine, mon, TH_CREATE);
//syncronize and join all threads

void	init_threads(t_simu *simu, t_monitor *mon)
{
	int	i;

	i = 0;
	mon->simu = simu;

	while (i < simu->seats)
	{
		mon->philo = simu->philosophers + i;
		handle_thread_op(&mon->philo->th_id, sim_routine, mon, TH_CREATE);
		// leak is here d^.^b / need thread join or detach
		i++;
	}
}

void	join_threads(t_simu *simu, t_monitor *mon)
{
	int	i;

	(void)mon;
	i = 0;
	while (i < simu->seats)
	{
		handle_thread_op(&(simu->philosophers + i)->th_id, NULL, NULL, TH_JOIN);
		i++;
	}
}

void	detach_threads(t_simu *simu)
{
	int	i;

	i = 0;
	while (i < simu->seats)
	{
		handle_thread_op(&(simu->philosophers + i)->th_id, NULL, NULL, TH_DETACH);
		i++;
	}
}
