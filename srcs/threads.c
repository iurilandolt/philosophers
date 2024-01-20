/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:42:15 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/20 18:50:02 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

bool is_dead(t_monitor *mon, t_philo *philo)
{
	long	elapsed;

	if (philo->dead)
		return (true);
	elapsed = get_time_mls() - philo->last_meal;
	if (elapsed > mon->simu->time_to_die)
	{
		print_status(mon, philo, M"has died."RST);
		return (true);
	}
	return (false);
}

bool	is_full(t_monitor *mon, t_philo *philo)
{
	if (philo->full)
		return (true);
	if (mon->simu->meal_goal <= 0)
		return (false);
	if (philo->meals >= mon->simu->meal_goal)
	{
		print_status(mon, philo, G"is full."RST);
		return (true);
	}
	return (false);
}

bool	lone_thread(t_monitor *mon, t_philo *philo)
{
	if (mon->simu->seats == 1)
	{
		ft_usleep(mon->simu, mon->simu->time_to_die);
		set_bool(&mon->mon_mtx, &mon->simu->sim_end, true);
		print_status(mon, philo, M"has died."RST);
		return (true);
	}
	return (false);
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
		usleep(10);
	}
}



