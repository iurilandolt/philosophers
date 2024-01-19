/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 00:15:44 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/19 00:49:03 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

bool	check_sim_end(t_simu *simu, t_monitor *mon)
{
	bool	status;

	handle_mutex_op(&mon->mtx, MTX_LOCK);
	status = simu->sim_end;
	handle_mutex_op(&mon->mtx, MTX_UNLOCK);
	return (status);
}

bool	set_sim_end(t_simu *simu, t_monitor *mon, bool value)
{
	bool	status;

	handle_mutex_op(&mon->mtx, MTX_LOCK);
	simu->sim_end = value;
	status = simu->sim_end;
	handle_mutex_op(&mon->mtx, MTX_UNLOCK);
	return (status);
}

/*
func for monitoring sim end
start monitor thread, join or detach?
might need extra thread to check dead philos.
iterate trhough philos, check last_meal time vs time_to_die // philo_is_dead()
set index 0 if philo died, +1 to dead count, if dead count == seats, sim_end = true
check if philo is full, if philo is full +1 full count, if full count == seats, sim_end = true
*/


void	*mon_routine(void *data)
{
	t_monitor	*mon;
	int			i;


	mon = (t_monitor *)data;
	while (!check_sim_end(mon->simu->sim_end, mon)) // might need mutex
	{
		i = 0;
		while (i < mon->simu->seats)
		{
			i++;
		}
	}
	return NULL;
}

void	init_monitor(t_monitor *mon)
{
	handle_thread_op(&mon->th_id, mon_routine, mon, TH_CREATE);
}
