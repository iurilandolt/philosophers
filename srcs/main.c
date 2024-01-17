/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 18:41:29 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/17 17:16:21 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

// cc -lpthread main.c
// cc -s to compile into assembly

/*
what is a process?
what is a thread?
what are race condition?
what is a mutex? / threads and mutex must be init and destroyed
lock mutex before chaging values, unlock after.
what is a conditional variable?
what is a signaling variable?

argv[1] -> number of philosophers
argv[2] -> time to die
argv[3] -> time to eat
argv[4] -> time to sleep
argv[5] -> meal goal

state change of a philosopher must be formatted as follows:
◦ timestamp_in_ms X has taken a fork
◦ timestamp_in_ms X is eating
◦ timestamp_in_ms X is sleeping
◦ timestamp_in_ms X is thinking
◦ timestamp_in_ms X died

displayed state message should not be mixed up with another message.

message announcing a philosopher died should be displayed no more than 10 ms
after the actual death of the philosopher.

*/
void	ft_error(char *str) // pass t_simu, free allocated memory if any existss
{
	printf("%s%s%s\n", R, str, RST);
	exit(EXIT_FAILURE);
}

void	*test_routine(void *data)
{
	printf("thread %d initialized.\n", *(int *)data);
	return NULL;
}

void	init_threads(t_simu *simu)
{
	int	i;

	i = 0;
	while (i < simu->seats)
	{
		handle_thread_op((simu->philosophers + i)->th_id, test_routine, &i, TH_CREATE);
		handle_mutex_op(&(simu->philosophers + i)->left->fork, MTX_INIT);
		handle_mutex_op(&(simu->philosophers + i)->left->fork, MTX_LOCK);
		usleep(30);
		i++;
	}
}
// main function has its own thread
// also need extra thread for monitoring philos
int main(int argc, char **argv)
{
	t_simu	simu;

	simu.philosophers = NULL;
	if (argc > 4 && argc < 7)
	{
		if (!init_sim_values(&simu, argc, argv))
			ft_error("Invalid arguments.");

		simu.philosophers = alloc_philos(&simu);
		simu.forks = alloc_forks(&simu); // also assigns forks, sequentially atm.
		init_threads(&simu);
		simu.sim_end = false;
		simu.sim_start = get_time_s();

		print_sim_values(&simu);




		free(simu.philosophers);
		free(simu.forks);
	}
	return (0);
}

