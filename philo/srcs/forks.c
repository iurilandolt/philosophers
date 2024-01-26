/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 00:13:24 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/25 12:19:46 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

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
		if (pthread_mutex_lock(&philo->right->mtx) != 0)
			ft_error("pthread_mutex_lock() failed.");
		print_status(philo, "has taken a fork");
	}
}

void	pickup_second_fork(t_philo *philo)
{
	if (philo->index % 2)
	{
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
