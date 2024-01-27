/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 01:37:47 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/27 13:19:19 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	ft_error(char *str)
{
	printf("%s%s%s\n", R, str, RST);
	exit(EXIT_FAILURE);
}

void	eat(t_philo *philo)
{
	if (get_bool(&philo->sim->mtx, &philo->sim->ended))
		return ;
	pickup_first_fork(philo);
	if (get_bool(&philo->sim->mtx, &philo->sim->ended)) //  || is_dead(philo)
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
	ft_usleep(philo->sim->time_to_eat);
	putdown_first_fork(philo);
	putdown_second_fork(philo);
}

void	rest(t_philo *philo)
{
	if (get_bool(&philo->sim->mtx, &philo->sim->ended))
		return ;
	print_status(philo, B"is sleeping"RST);
	ft_usleep(philo->sim->time_to_sleep);
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
	while (1)
	{

		if (is_full(philo))
			break ;
		if (get_bool(&philo->sim->mtx, &philo->sim->ended))
			break ;

		//if (is_dead(philo))
		//	break ;
		print_status(philo, Y"is thinking"RST);
		if (philo->sim->seats % 2 && philo->index % 2)
			ft_usleep(5);
		if (get_bool(&philo->sim->mtx, &philo->sim->ended))
			break ;
		//if (is_dead(philo))
			//break ;
		eat(philo);
		//if (get_bool(&philo->sim->mtx, &philo->sim->ended))
		//	break ;
		rest(philo);
	}
	return (NULL);
}
