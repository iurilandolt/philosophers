/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 22:31:41 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/26 20:04:09 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

t_philo	*alloc_philos(t_sim *sim)
{
	t_philo	*new;
	int		i;

	new = (t_philo *)malloc(sizeof(t_philo) * sim->seats);
	if (!new)
		return (NULL);
	i = 0;
	while (i < sim->seats)
	{
		(new + i)->sim = (void *)sim;
		(new + i)->index = i + 1;
		(new + i)->meals = 0;
		(new + i)->limit = sim->goal;
		(new + i)->full = false;
		(new + i)->last_meal = 0;
		(new + i)->dead = false;
		(new + i)->left = NULL;
		(new + i)->right = NULL;
		(new + i)->lspan = sim->time_to_die;
		if (pthread_mutex_init(&(new + i)->mtx, NULL) != 0)
			ft_error("pthread_mutex_init() failed.");
		i++;
	}
	return (new);
}

t_fork	*alloc_forks(t_sim *sim)
{
	t_fork	*new;
	int		i;

	new = (t_fork *)malloc(sizeof(t_fork) * sim->seats);
	if (!new)
		return (NULL);
	i = 0;
	while (i < sim->seats)
	{
		(new + i)->index = i;
		(sim->philosophers + i)->left = new + i;
		if (pthread_mutex_init(&(sim->philosophers + i)->left->mtx, NULL) != 0)
			ft_error("pthread_mutex_init() failed.");
		(sim->philosophers + i)->right = new + ((i + 1) % sim->seats);
		i++;
	}
	return (new);
}

/*
		(new + i)->index = i;
		if ((sim->philosophers + i)->index % 2)
		{
			(sim->philosophers + i)->left = new + i;
			(sim->philosophers + i)->right = new + ((i + 1) % sim->seats);
			if (pthread_mutex_init(&(sim->philosophers + i)->left->mtx, NULL) != 0)
				ft_error("pthread_mutex_init() failed.");
		}
		else
		{
			(sim->philosophers + i)->left = new + ((i + 1) % sim->seats);
			(sim->philosophers + i)->right = new + i;
			if (pthread_mutex_init(&(sim->philosophers + i)->right->mtx, NULL) != 0)
				ft_error("pthread_mutex_init() failed.");
		}
*/

void	*monitor(void *data)
{
	t_sim	*sim;
	int		i;
	int 	full;

	sim = (t_sim *)data;
	full = 0;
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

void	threads_create(t_sim *sim)
{
	int		i;
	pthread_t	moni;



	i = 0;
	while (i < sim->seats)
	{
		if (pthread_create(&(sim->philosophers + i)->th_id, NULL, philosopher,
				(void *)(sim->philosophers + i)) != 0)
			ft_error("pthread_create() failed.");
		i++;
	}
	if (pthread_create(&moni, NULL, monitor, (void *)sim) != 0)
		ft_error("pthread_create() failed.");
	if (pthread_join(moni, NULL) != 0)
		ft_error("pthread_join() failed.");
	i = 0;
	while (i < sim->seats)
	{
		if (pthread_join((sim->philosophers + i)->th_id, NULL) != 0)
			ft_error("pthread_join() failed.");
		i++;
	}


}

void	startup(t_sim *sim)
{
	sim->ended = false;
	sim->philosophers = alloc_philos(sim);
	sim->forks = alloc_forks(sim);
	sim->start = get_time();
	if (pthread_mutex_init(&sim->mtx, NULL) != 0)
		ft_error("pthread_mutex_init() failed.");
}

void	cleanup(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->seats)
	{
		if (pthread_mutex_destroy(&(sim->philosophers + i)->mtx) != 0)
			ft_error("pthread_mutex_destroy() failed.");
		if (pthread_mutex_destroy(&(sim->philosophers + i)->left->mtx) != 0)
			ft_error("pthread_mutex_destroy() failed.");
		i++;
	}
	pthread_mutex_destroy(&sim->mtx);
	free(sim->philosophers);
	free(sim->forks);
}


/*


void	*monitor(void *data)
{
	t_sim	*sim;
	int		i;

	sim = (t_sim *)data;
	while (1)
	{

		i = 0;
		while (i < sim->seats)
		{
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

void	threads_create(t_sim *sim)
{
	int		i;
	pthread_t	moni;

	i = 0;
	while (i < sim->seats)
	{
		if (pthread_create(&(sim->philosophers + i)->th_id, NULL, philosopher,
				(void *)(sim->philosophers + i)) != 0)
			ft_error("pthread_create() failed.");
		i++;
	}
	if (pthread_create(&moni, NULL, monitor, (void *)sim) != 0)
		ft_error("pthread_create() failed.");
	i = 0;
	while (i < sim->seats)
	{
		if (pthread_join((sim->philosophers + i)->th_id, NULL) != 0)
			ft_error("pthread_join() failed.");
		i++;
	}
	if (pthread_join(moni, NULL) != 0)
		ft_error("pthread_join() failed.");
}
*/
