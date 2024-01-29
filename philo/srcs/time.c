/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 15:12:55 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/29 16:32:36 by rlandolt         ###   ########.fr       */
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

void	*monitor(void *data)
{
	t_sim	*sim;
	int		i;
	int		full;

	sim = (t_sim *)data;
	full = 0;
	ft_usleep(sim->seats);
	while (1)
	{
		i = 0;
		while (i < sim->seats)
		{
			if (get_bool(&sim->mtx, &sim->ended))
				return (NULL);
			if (get_bool(&sim->mtx, &(sim->philosophers + i)->full))
				full++;
			if (full == sim->seats)
				return (NULL);
			if (is_dead(sim->philosophers + i))
			{
				if (pthread_mutex_lock(&sim->mtx) != 0)
					ft_error("pthread_mutex_lock() failed.");
				sim->ended = true;
				if (pthread_mutex_unlock(&sim->mtx) != 0)
					ft_error("pthread_mutex_unlock() failed.");
				return (NULL);
			}
			i++;
		}
	}
}
