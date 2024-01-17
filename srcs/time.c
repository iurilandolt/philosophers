/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 15:12:55 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/17 22:18:20 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/*add custom usleep func?*/
long	get_time_s(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		ft_error("gettimeofday failed.");
	return (tv.tv_sec + tv.tv_usec / 1e6);
}

long	get_time_mls(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		ft_error("gettimeofday failed.");
	return ((tv.tv_sec * 1e3) + (tv.tv_sec / 1e3));
}

long	get_time_mcs(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		ft_error("gettimeofday failed.");
	return (tv.tv_sec * 1e6 + tv.tv_sec * 1e6);
}

// spinlock ?


