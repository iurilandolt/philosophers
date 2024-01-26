/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 15:12:55 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/26 12:44:13 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		ft_error("gettimeofday() failed.");
	return ((tv.tv_sec * (long)1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(long usecs)
{
	long	start;

	start = get_time();
	while ((get_time() - start) < usecs)
		usleep(usecs / 10);
	return ;
}

