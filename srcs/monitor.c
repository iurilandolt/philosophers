/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 00:15:44 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/20 13:07:28 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void set_bool(pthread_mutex_t *mtx, bool *var, bool value)
{
	handle_mutex_op(mtx, MTX_LOCK);
	*var = value;
	handle_mutex_op(mtx, MTX_UNLOCK);
}

bool get_bool(pthread_mutex_t *mtx, bool *value)
{
	bool	ret;

	handle_mutex_op(mtx, MTX_LOCK);
	ret = *value;
	handle_mutex_op(mtx, MTX_UNLOCK);
	return (ret);
}

void	print_status(t_monitor *mon, t_philo *philo, char *status)
{
	long	elapsed;

	elapsed = get_time_mls() - mon->simu->sim_srt;
	handle_mutex_op(&mon->write_mtx, MTX_LOCK);
	printf(W"%-6ldms"RST" : philo#%d : %s\n", elapsed, philo->index, status);
	handle_mutex_op(&mon->write_mtx, MTX_UNLOCK);
}


void	init_monitor(t_simu *simu, t_monitor *mon)
{
	mon->simu = simu;

	handle_mutex_op(&mon->mon_mtx, MTX_INIT);
	handle_mutex_op(&mon->write_mtx, MTX_INIT);
	handle_mutex_op(&mon->loop_mtx, MTX_INIT);


	handle_thread_op(&mon->th_id, mon_routine, mon, TH_CREATE);
	handle_thread_op(&mon->th_id, NULL, NULL, TH_DETACH);
}


