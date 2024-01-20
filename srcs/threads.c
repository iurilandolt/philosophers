/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:42:15 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/20 17:10:36 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

bool is_dead(t_monitor *mon, t_philo *philo)
{
	long	elapsed;

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
	if (mon->simu->meal_goal <= 0)
		return (false);
	if (philo->meals >= mon->simu->meal_goal)
	{
		print_status(mon, philo, G"is full."RST);
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
	handle_thread_op(&mon->th_id, NULL, NULL, TH_DETACH);
	return NULL;
}

void	eat(t_monitor *mon, t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (philo->index % 2) // odd -> picks left fork first
	{
		first = &philo->left->fork;
		second = &philo->right->fork;
	}
	else // even -> picks right fork first
	{
		first = &philo->right->fork;
		second = &philo->left->fork;
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

bool	lone_philo(t_monitor *mon, t_philo *philo)
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
		if (lone_philo(mon, philo))
			break;
		philo->full = is_full(mon, philo);
		philo->dead = is_dead(mon, philo);
		if (philo->full || philo->dead)
			break ;

		print_status(mon, philo, Y"is thinking."RST);
		ft_usleep(mon->simu, mon->simu->seats * 100);

		eat(mon, philo); // tryna eat

		print_status(mon, philo, B"is sleeping."RST);
		ft_usleep(mon->simu, mon->simu->time_to_sleep);

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
		usleep(10);
	}
}



