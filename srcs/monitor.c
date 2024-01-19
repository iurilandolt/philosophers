/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 00:15:44 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/19 12:25:09 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

bool	check_sim_end(t_simu *simu, t_monitor *mon)
{
	bool	status;

	handle_mutex_op(&mon->loop_mtx, MTX_LOCK);
	status = simu->sim_end;
	handle_mutex_op(&mon->loop_mtx, MTX_UNLOCK);
	return (status);
}

bool	set_sim_end(t_simu *simu, t_monitor *mon, bool value)
{
	bool	status;

	handle_mutex_op(&mon->loop_mtx, MTX_LOCK);
	simu->sim_end = value;
	status = simu->sim_end;
	handle_mutex_op(&mon->loop_mtx, MTX_UNLOCK);
	return (status);
}


bool get_bool(pthread_mutex_t *mtx, bool *value)
{
	bool	ret;

	handle_mutex_op(mtx, MTX_LOCK);
	ret = *value;
	handle_mutex_op(mtx, MTX_UNLOCK);
	return (ret);
}

int	get_int(pthread_mutex_t *mtx, int *value)
{
	int	ret;

	handle_mutex_op(mtx, MTX_LOCK);
	ret = *value;
	handle_mutex_op(mtx, MTX_UNLOCK);
	return (ret);
}
/*
func for monitoring sim end
start monitor thread, join or detach?
might need extra thread to check dead philos.
iterate trhough philos, check last_meal time vs time_to_die // philo_is_dead()
set index 0 if philo died, +1 to dead count, if dead count == seats, sim_end = true
check if philo is full, if philo is full +1 full count, if full count == seats, sim_end = true
*/
int	philo_is_dead(t_philo *philo, int time_to_die)
{
	if (get_time_mls() - philo->last_meal > time_to_die) // get time mcs?
		return (1);
	return (0);
}

void	*mon_routine(void *data)
{
	t_monitor	*mon;
	int			i;
	//int			full;
	//int 		dead;

	mon = (t_monitor *)data;
	i = 0;
	handle_mutex_op(&mon->mon_mtx, MTX_INIT);
	handle_mutex_op(&mon->loop_mtx, MTX_INIT);

	while (!check_sim_end(mon->simu, mon))
	{
		while (i < mon->simu->seats)
		{
			if ((mon->simu->philosophers + i)->index == 3)
			{
				set_sim_end(mon->simu, mon, true);
				break;
			}
			printf("philo %d is alive\n", i + 1);
			i++;
		}
		printf("sim has ended %s\n", mon->simu->sim_end ? "true" : "false");
	}


	return NULL;
}

void	init_monitor(t_simu *simu, t_monitor *mon)
{
	mon->simu = simu;
	handle_thread_op(&mon->th_id, mon_routine, mon, TH_CREATE);
	handle_thread_op(&mon->th_id, NULL, NULL, TH_DETACH);
}

/*

	handle_mutex_op(&mon->loop_mtx, MTX_INIT);
	wait_for_threads(mon->simu);
	while (!check_sim_end(mon->simu, mon)) // might need loop_mutex
	{
		i = 0;
		while (i < mon->simu->seats)
		{
			if (full == mon->simu->seats || dead == mon->simu->seats)
				break ;
			mon->philo = mon->simu->philosophers + i;
			if (mon->philo->meals >= mon->simu->meal_goal)
			{
				mon->philo->full = true;
				mon->philo->index = 0;
				full++;
			}
			if (philo_is_dead(mon->philo, mon->simu->time_to_die))
			{
				mon->philo->index = -1;
				dead++;
			}
			i++;
		}
		set_sim_end(mon->simu, mon, true);
	}

*/
