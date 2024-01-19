/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:42:15 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/19 12:56:28 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"



//spinlock / busy waiting

void	wait_for_threads(t_simu *simu)
{
	while (!simu->sim_rdy) // race condition ?
		;
}

void	*sim_routine(void *data)
{
	t_monitor	*mon;

	mon = (t_monitor *)data;
	wait_for_threads(mon->simu);

	mon->th_id = pthread_self();
	printf("Philo Thread ID: %lu is ready.\n", (unsigned long) mon->th_id);
	return NULL;
}


//handle_thread_op(mon->th_id, mon_routine, mon, TH_CREATE);
// init monitor here? make monitor wait for threads?
// monitor join or detach?

void	init_threads(t_simu *simu, t_monitor *mon)
{
	int	i;

	i = 0;
	mon->simu = simu;

	while (i < simu->seats)
	{
		mon->philo = simu->philosophers + i;
		handle_thread_op(&mon->philo->th_id, sim_routine, mon, TH_CREATE);
		mon->philo->meals = 0;
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
