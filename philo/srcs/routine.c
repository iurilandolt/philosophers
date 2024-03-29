/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 01:37:47 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/29 23:26:06 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	eat(t_philo *philo)
{
	if (get_bool(&philo->sim->mtx, &philo->sim->ended))
		return ;
	pickup_first_fork(philo);
	if (get_bool(&philo->sim->mtx, &philo->sim->ended))
	{
		putdown_first_fork(philo);
		return ;
	}
	pickup_second_fork(philo);
	if (pthread_mutex_lock(&philo->sim->mtx) != 0)
		ft_error("pthread_mutex_lock() failed.");
	philo->meals++;
	philo->last_meal = get_time();
	if (pthread_mutex_unlock(&philo->sim->mtx) != 0)
		ft_error("pthread_mutex_unlock() failed.");
	print_status(philo, G"is eating"RST);
	ft_sleep(philo->sim->time_to_eat);
	putdown_first_fork(philo);
	putdown_second_fork(philo);
}

void	rest(t_philo *philo)
{
	if (get_bool(&philo->sim->mtx, &philo->sim->ended))
		return ;
	print_status(philo, B"is sleeping"RST);
	ft_sleep(philo->sim->time_to_sleep);
}

void	think(t_philo *philo)
{
	int	think;

	if (get_bool(&philo->sim->mtx, &philo->sim->ended))
		return ;
	print_status(philo, Y"is thinking"RST);
	if (philo->sim->seats % 2 == 0)
		return ;
	think = philo->sim->time_to_eat * 2 - philo->sim->time_to_sleep;
	if (think < 0)
		think = 0;
	ft_sleep(think * 0.40);
}

void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	pthread_mutex_lock(&philo->sim->mtx);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->sim->mtx);
	if (lone_thread(philo))
		return (NULL);
	if (philo->index % 2)
		ft_sleep(philo->sim->time_to_eat);
	while (1)
	{
		if (is_full(philo))
			break ;
		if (get_bool(&philo->sim->mtx, &philo->sim->ended))
			break ;
		eat(philo);
		rest(philo);
		think(philo);
	}
	return (NULL);
}

/*
		//print_status(philo, Y"is thinking"RST);
		//if (philo->sim->seats % 2 && philo->index % 2)
		//	ft_sleep(5);

		void	update_priority(t_philo *philo)
		{
			long	elapsed;
			long	threshold;

			if (pthread_mutex_lock(&philo->mtx) != 0)
				ft_error("pthread_mutex_unlock() failed.");
			elapsed = get_time() - philo->last_meal;
			if (pthread_mutex_unlock(&philo->mtx) != 0)
				ft_error("pthread_mutex_lock() failed.");
			threshold = philo->lspan * 0.30;
			if (elapsed <= threshold)
				ft_sleep(philo->lspan * 0.30);
			else
				return ;
		}
*/
