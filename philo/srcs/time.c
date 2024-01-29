/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 15:12:55 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/29 17:07:48 by rlandolt         ###   ########.fr       */
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

	sim = (t_sim *)data;
	sim->served = 0;
	while (1)
	{
		i = -1;
		while (++i < sim->seats)
		{
			if (get_bool(&sim->mtx, &sim->ended))
				return (NULL);
			if (get_bool(&sim->mtx, &(sim->philosophers + i)->full))
				sim->served++;
			if (sim->served == sim->seats)
				return (NULL);
			if (is_dead(sim->philosophers + i))
			{
				pthread_mutex_lock(&sim->mtx);
				sim->ended = true;
				pthread_mutex_unlock(&sim->mtx);
				return (NULL);
			}
		}
	}
}
