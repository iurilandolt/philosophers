/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:42:15 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/20 12:53:57 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

bool is_dead(t_monitor *mon, t_philo *philo)
{
	long	elapsed;

	elapsed = get_time_mls() - philo->last_meal;
	if (elapsed > mon->simu->time_to_die)
	{
		print_status(mon, philo, "has died.");
		return (true);
	}
	return (false);
}

bool	is_full(t_monitor *mon, t_philo *philo)
{
	if (mon->simu->meal_goal <= 0)
		return (false);
	if (philo->meals >= mon->simu->meal_goal)
	{
		print_status(mon, philo, "is full.");
		return (true);
	}
	return (false);
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

		philo->dead = is_dead(mon, philo);
		if (philo->full || philo->dead)
			break ;

		print_status(mon, philo, "is thinking.");

		//eat
		//sleep
		//think

	}
	//philo->th_id = pthread_self();
	//printf("Philo Thread ID: %lu is gone.\n", (unsigned long) philo->th_id);
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
			mon->philo->full = false;
			mon->philo->dead = false;
			handle_thread_op(&mon->philo->th_id, sim_routine, mon, TH_CREATE);
		}
		else if (op == TH_JOIN)
			handle_thread_op(&mon->philo->th_id, NULL, NULL, TH_JOIN);
		else if (op == TH_DETACH)
			handle_thread_op(&mon->philo->th_id, NULL, NULL, TH_DETACH);
		i++;
		//usleep(1);
	}
}



