/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 00:15:44 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/27 13:11:48 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	print_status(t_philo *philo, char *status)
{
	long	elapsed;

	elapsed = get_time() - philo->sim->start;
	if (get_bool(&philo->sim->mtx, &philo->sim->ended))
		return ;
	pthread_mutex_lock(&philo->sim->mtx);
	printf(W"%ld"RST"  "C"%d"RST" %s\n", elapsed, philo->index, status);
	pthread_mutex_unlock(&philo->sim->mtx);
}

bool	get_bool(pthread_mutex_t *mtx, bool *value)
{
	bool	ret;

	pthread_mutex_lock(mtx);
	ret = *value;
	pthread_mutex_unlock(mtx);
	return (ret);
}

bool	is_dead(t_philo *philo)
{
	long	elapsed;

	if (pthread_mutex_lock(&philo->sim->mtx) != 0)
		ft_error("pthread_mutex_init() failed.");
	elapsed = get_time() - philo->last_meal;
	if (pthread_mutex_unlock(&philo->sim->mtx) != 0)
		ft_error("pthread_mutex_init() failed.");
	if (elapsed < philo->lspan)
		return (false);
	else if (get_bool(&philo->sim->mtx, &philo->sim->ended) || philo->full)
		return (false);
	else
	{
		if (pthread_mutex_lock(&philo->mtx) != 0)
			ft_error("pthread_mutex_lock() failed.");
		philo->dead = true;
		if (pthread_mutex_unlock(&philo->mtx) != 0)
			ft_error("pthread_mutex_unlock() failed.");
		print_status(philo, M"died"RST);
	}
	return (true);
}

bool	is_full(t_philo *philo)
{
	if (philo->meals == philo->limit)
	{
		if (pthread_mutex_lock(&philo->sim->mtx) != 0)
			ft_error("pthread_mutex_lock() failed.");
		philo->full = true;
		if (pthread_mutex_unlock(&philo->sim->mtx) != 0)
			ft_error("pthread_mutex_unlock() failed.");
		return (true);
	}
	return (false);
}

bool	lone_thread(t_philo *philo)
{
	if (philo->sim->seats == 1)
	{
		print_status(philo, "is thinking");
		print_status(philo, "has picked up a fork");
		ft_usleep(philo->lspan);
		print_status(philo, M"has died."RST);

		if (pthread_mutex_lock(&philo->sim->mtx) != 0)
			ft_error("pthread_mutex_lock() failed.");
		philo->sim->ended = true;
		philo->dead = true;
		if (pthread_mutex_unlock(&philo->sim->mtx) != 0)
			ft_error("pthread_mutex_unlock() failed.");

		return (true);
	}
	return (false);
}


static void print_philo_values(t_philo *philo)
{
	printf("philo: %d\nmeals: %d\n", philo->index, philo->meals);
	printf("full: %s\n", philo->full ? "true" : "false");
	printf("dead: %s\n", philo->dead ? "true" : "false");
	printf("last_meal: %ld\n", philo->last_meal);
	printf("left fork: %d\n", philo->left->index);
	printf("right fork: %d\n\n", philo->right->index);
}

void	print_sim_values(t_sim *sim)
{
	int	i;
	int rip;
	int full;
	int meals;

	i = 0;
	rip = 0;
	full = 0;
	meals = 0;
	printf("-> seats: %d\n", sim->seats);
	printf("-> time_to_die: %d\n", sim->time_to_die);
	printf("-> time_to_eat: %d\n", sim->time_to_eat);
	printf("-> time_to_sleep: %d\n", sim->time_to_sleep);
	printf("-> meal_goal: %d\n", sim->goal);
	printf("-> sim started: %ld\n", sim->start);
	printf("-> sim ended: %s\n\n", sim->ended ? "true" : "false");
	while (i < sim->seats)
	{
		if ((sim->philosophers + i)->dead)
			rip++;
		if ((sim->philosophers + i)->full)
			full++;
		meals += (sim->philosophers + i)->meals;
		print_philo_values((sim->philosophers + i));
		i++;
	}
	printf("-> Dead Philosophers (RIP): %d.\n", rip);
	printf("-> Philosophers with full stomachs: %d.\n", full);
	printf("-> Meals Served: %d\n", meals);
}

