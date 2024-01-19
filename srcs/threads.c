/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:42:15 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/19 14:46:26 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

//spinlock / busy waiting
/*
void	wait_for_threads(t_simu *simu)
{
	while (!simu->sim_rdy) // race condition ?
		;
}
*/
bool get_bool(pthread_mutex_t *mtx, bool *value)
{
	bool	ret;

	handle_mutex_op(mtx, MTX_LOCK);
	ret = *value;
	handle_mutex_op(mtx, MTX_UNLOCK);
	return (ret);
}

void	wait_for_threads(t_simu *simu, t_monitor *mon)
{
	while (!get_bool(&mon->mon_mtx, &simu->sim_rdy))
		;
}

void	*sim_routine(void *data)
{
	t_monitor	*mon;

	mon = (t_monitor *)data;
	wait_for_threads(mon->simu, mon);

	// philo routine goes here ?//

	mon->th_id = pthread_self();
	printf("Philo Thread ID: %lu is ready.\n", (unsigned long) mon->th_id);
	return NULL;
}

void	all_threads_do(t_simu *simu, t_monitor *mon, t_thcode op)
{
	int	i;

	i = 0;
	mon->simu = simu;
	while (i < simu->seats)
	{
		mon->philo = simu->philosophers + i;
		if (op == TH_CREATE)
		{
			mon->philo->meals = 0;
			handle_thread_op(&mon->philo->th_id, sim_routine, mon, TH_CREATE);
		}
		else if (op == TH_JOIN)
			handle_thread_op(&mon->philo->th_id, NULL, NULL, TH_JOIN);
		else if (op == TH_DETACH)
			handle_thread_op(&mon->philo->th_id, NULL, NULL, TH_DETACH);
		i++;
	}
}



