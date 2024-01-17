/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 18:41:29 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/17 20:02:01 by rlandolt         ###   ########.fr       */
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


void	*sim_routine(void *data) // mby pass t_simu :(
{
	t_philo	*philo;

	philo = (t_philo *)data;
	//wait all threads!
	printf("thread initialized by philo %d\n", philo->index);
	return NULL;
}

void	init_threads(t_simu *simu)
{
	int	i;

	i = 0;
	handle_mutex_op(&simu->mtx, MTX_INIT);
	while (i < simu->seats)
	{
		handle_thread_op((simu->philosophers + i)->th_id, sim_routine,
			(simu->philosophers + i), TH_CREATE); // leak is here d^.^b / need thread join or detach
		i++;
	}
	handle_mutex_op(&simu->mtx, MTX_LOCK);
	simu->sim_rdy = true;
	handle_mutex_op(&simu->mtx, MTX_UNLOCK);
	//set_bool(&simu->mtx, &simu->sim_rdy, true);
}
// main function has its own thread
// also need extra thread for monitoring philos

// if only 1 philo?
//syncronize and join?

void *start_sim(t_simu *simu)
{
	simu->sim_rdy = false;
	simu->sim_end = false;
	simu->philosophers = alloc_philos(simu);
	simu->forks = alloc_forks(simu);
	if (!simu->meal_goal)
		return (NULL); // clean up and exit
	else if (!simu->seats)
		return (NULL); // how handle 1 philo?
	else
		init_threads(simu); // leak is here d^.^b
	simu->sim_srt = get_time_s();
	return (NULL);
}
int main(int argc, char **argv)
{
	t_simu	simu;

	if (argc > 4 && argc < 7)
	{
		if (!init_sim_values(&simu, argc, argv))
			ft_error("Invalid arguments.");
		/*
		simu.philosophers = alloc_philos(&simu);
		simu.forks = alloc_forks(&simu);
		simu.sim_rdy = false;
		simu.sim_end = false;
		simu.sim_srt = get_time_s();
		*/

		start_sim(&simu); // leaking

		print_sim_values(&simu);




		free(simu.philosophers);
		free(simu.forks);
	}
	return (0);
}

