/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 15:12:55 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/19 15:06:24 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

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
	return ((tv.tv_sec * 1e6) + tv.tv_usec);
}

void	ft_usleep(t_simu *simu, long time)
{
	long	start;
	long	remain;
	long	elapsed;

	start = get_time_mcs();
	while (get_time_mcs() - start < time)
	{
		if (simu->sim_end) // race condition ?
			break ;
		elapsed = get_time_mcs() - start;
		remain = time - elapsed;
		if (remain > 1e3)
			usleep(remain / 2);
		else
		{
			while (get_time_mcs() - start < time)
				;
		}
	}
}
