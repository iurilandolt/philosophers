/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 15:12:55 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/20 01:04:55 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
//spinlock
void	wait_for_threads(t_simu *simu, t_monitor *mon)
{
	while (!get_bool(&mon->mon_mtx, &simu->sim_rdy))
		;
}

long get_time_mls(void) {
	struct timeval tv;

	if (gettimeofday(&tv, NULL))
		ft_error("gettimeofday failed.");
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
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
