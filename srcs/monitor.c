/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 00:15:44 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/19 15:23:05 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/*
func for monitoring sim end

iterate trhough philos, check last_meal time vs time_to_die // philo_is_dead()

set index 0 if philo died, +1 to dead count, if dead count == seats, sim_end = true

check if philo is full, if philo is full +1 full count, if full count == seats, sim_end = true
*/

void	*mon_routine(void *data)
{
	t_monitor	*mon;


	mon = (t_monitor *)data;
	wait_for_threads(mon->simu, mon); // monitor might not need to wait for threads
	// monitor routine goes here ?//

	mon->th_id = pthread_self();
	printf("Monit Thread ID: %lu is ready.\n", (unsigned long) mon->th_id);

	return NULL;
}

void	init_monitor(t_simu *simu, t_monitor *mon)
{
	mon->simu = simu;

	handle_mutex_op(&mon->mon_mtx, MTX_INIT);
	handle_mutex_op(&mon->write_mtx, MTX_INIT);

	handle_thread_op(&mon->th_id, mon_routine, mon, TH_CREATE);
	handle_thread_op(&mon->th_id, NULL, NULL, TH_DETACH);
}


