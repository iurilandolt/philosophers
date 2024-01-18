/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 18:41:29 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/18 19:24:35 by rlandolt         ###   ########.fr       */
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

void set_bool(pthread_mutex_t *mtx, bool *var, bool value)
{
	handle_mutex_op(mtx, MTX_LOCK);
	*var = value;
	handle_mutex_op(mtx, MTX_UNLOCK);
}

bool get_bool(pthread_mutex_t *mtx, bool *value)
{
	bool	ret;

	handle_mutex_op(mtx, MTX_LOCK);
	ret = *value;
	handle_mutex_op(mtx, MTX_UNLOCK);
	return (ret);
}

void	wait_for_threads(t_simu *simu, t_monitor *mon)
{
	while (!get_bool(&mon->mtx, &simu->sim_rdy))
		;
}

*/
void	ft_error(char *str) // pass t_simu, free allocated memory if any existss
{
	printf("%s%s%s\n", R, str, RST);
	exit(EXIT_FAILURE);
}
/*func for monitoring sim end*/
bool	sim_end(t_simu *simu, t_monitor *mon)
{
	bool	status;

	handle_mutex_op(&mon->mtx, MTX_LOCK);
	status = simu->sim_end;
	handle_mutex_op(&mon->mtx, MTX_UNLOCK);
	return (status);
}

/*
if any of the arguments is zero
init_sim_values & validate_input() will stop the program from reacing this point

		pthread_t thread_id = pthread_self();
		printf("Thread ID: %lu\n", (unsigned long) thread_id);
*/

void *start_sim(t_simu *simu, t_monitor *mon)
{
	simu->sim_rdy = false;
	simu->sim_end = false;
	simu->philosophers = alloc_philos(simu);
	simu->forks = alloc_forks(simu);
	handle_mutex_op(&mon->mtx, MTX_INIT);

	init_threads(simu, mon); // leak is here d^.^b

	handle_mutex_op(&mon->mtx, MTX_LOCK);
	simu->sim_rdy = true;
	simu->sim_srt = get_time_mls();
	handle_mutex_op(&mon->mtx, MTX_UNLOCK);

	join_threads(simu, mon);

	printf("-> sim ready: %s @ %ld\n", simu->sim_rdy ? "true" : "false", simu->sim_srt);

	return (NULL);
}

// if only 1 philo?
// main function has its own thread.
int main(int argc, char **argv)
{
	t_simu		simu;
	t_monitor	mon;

	if (argc > 4 && argc < 7)
	{
		if (!init_sim_values(&simu, argc, argv))
			ft_error("Invalid arguments.");
		start_sim(&simu, &mon); // leaking

		print_sim_values(&simu);

		free(simu.philosophers);
		free(simu.forks);
	}
	return (0);
}

