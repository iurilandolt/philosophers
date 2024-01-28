/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 00:13:24 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/28 22:09:17 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"


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
		ft_usleep(philo->lspan * 0.30);
	else
		return ;
}

void	pickup_first_fork(t_philo *philo)
{
	if (philo->index % 2)
	{
		if (pthread_mutex_lock(&philo->left->mtx) != 0)
			ft_error("pthread_mutex_lock() failed.");
		print_status(philo, "has taken a fork");
	}
	else
	{
		update_priority(philo);
		if (pthread_mutex_lock(&philo->right->mtx) != 0)
			ft_error("pthread_mutex_lock() failed.");
		print_status(philo, "has taken a fork");
	}
}

void	pickup_second_fork(t_philo *philo)
{
	if (philo->index % 2)
	{
		update_priority(philo);
		if (pthread_mutex_lock(&philo->right->mtx) != 0)
			ft_error("pthread_mutex_lock() failed.");
		print_status(philo, "has taken a fork");
	}
	else
	{
		if (pthread_mutex_lock(&philo->left->mtx) != 0)
			ft_error("pthread_mutex_lock() failed.");
		print_status(philo, "has taken a fork");
	}
}

void	putdown_first_fork(t_philo *philo)
{
	if (philo->index % 2)
	{
		if (pthread_mutex_unlock(&philo->left->mtx) != 0)
			ft_error("pthread_mutex_unlock() failed.");
	}
	else
	{
		if (pthread_mutex_unlock(&philo->right->mtx) != 0)
			ft_error("pthread_mutex_unlock() failed.");
	}
}

void	putdown_second_fork(t_philo *philo)
{
	if (philo->index % 2)
	{
		if (pthread_mutex_unlock(&philo->right->mtx) != 0)
			ft_error("pthread_mutex_unlock() failed.");
	}
	else
	{
		if (pthread_mutex_unlock(&philo->left->mtx) != 0)
			ft_error("pthread_mutex_unlock() failed.");
	}
}
