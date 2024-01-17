/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 13:15:11 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/17 10:52:57 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*test_routine(void *data)
{
	int	*i;

	i = (int *)data;
	printf("test %d ok\n", *(int *)data);
	return NULL;
}

t_philo	*alloc_philos(t_simu *simu)
{
	t_philo	*new;
	int		i;

	new = (t_philo *)malloc(sizeof(t_philo) * simu->seats);
	if (!new)
		return (NULL);
	i = 0;
	while (i < simu->seats)
	{
		(new + i)->index = i + 1;
		(new + i)->meals = 0;
		(new + i)->full = false;
		(new + i)->last_meal = 0;
		i++;
	}
	return (new);
}

t_fork	*alloc_forks(t_simu *simu)
{
	t_fork	*new;
	int		i;

	new = (t_fork *)malloc(sizeof(t_fork) * simu->seats);
	if (!new)
		return (NULL);
	i = 0;
	while (i < simu->seats)
	{
		(new + i)->index = i + 1;
		(simu->philosophers + i)->left = new + i;
		(simu->philosophers + i)->right = new + ((i + 1) % simu->seats);
		i++;
	}
	return (new);
}

static void print_philo_values(t_philo *philo)
{
	printf("philo: %d meals: %d ", philo->index, philo->meals);
	printf("full: %s\n", philo->full ? "true" : "false");
	printf("last_meal: %ld\n", philo->last_meal);
	printf("left fork: %d\n", philo->left->index);
	printf("right fork: %d\n", philo->right->index);
}

void	print_sim_values(t_simu *simu)
{
	int	i;

	i = 0;
	printf("-> seats: %d\n", simu->seats);
	printf("-> time_to_die: %d\n", simu->time_to_die);
	printf("-> time_to_eat: %d\n", simu->time_to_eat);
	printf("-> time_to_sleep: %d\n", simu->time_to_sleep);
	printf("-> meal_goal: %d\n", simu->meal_goal);
	while (i < simu->seats)
	{
		print_philo_values((simu->philosophers + i));
		i++;
	}
}
