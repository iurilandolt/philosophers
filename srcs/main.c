/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 18:41:29 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/17 22:48:58 by rlandolt         ###   ########.fr       */
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

void	wait_for_threads(t_simu *simu)
{
	while (!simu->sim_rdy)
		;
}
/*
void	*sim_routine(void *data) // mby pass t_simu :(
{
	t_philo	*philo;
	//t_simu	*simu;

	philo = (t_philo *)data;
	//simu = (t_simu *)data;
	//wait_for_threads(philo->simu);
	printf("thread initialized by philo %d\n", philo->index);
	return NULL;
}
*/
void	*sim_routine(void *data) // mby pass t_simu :(
{
	t_monitor	*mon;

	mon = (t_monitor *)data;
	wait_for_threads(mon->simu);
	return NULL;
}

void	init_threads(t_simu *simu, t_monitor *mon)
{
	int	i;

	i = 0;
	mon->simu = simu;
	handle_mutex_op(&mon->mtx, MTX_INIT);
	while (i < simu->seats)
	{
		mon->philo = simu->philosophers + i;
		handle_thread_op(mon->philo->th_id, sim_routine,
			mon, TH_CREATE); // leak is here d^.^b / need thread join or detach
		i++;
	}
	handle_mutex_op(&mon->mtx, MTX_LOCK);
	simu->sim_rdy = true;
	simu->sim_srt = get_time_s();
	printf("-> sim ready: %s @ %ld\n", simu->sim_rdy ? "true" : "false", simu->sim_srt);
	handle_mutex_op(&mon->mtx, MTX_UNLOCK);
	//set_bool(&simu->mtx, &simu->sim_rdy, true);
}
// main function has its own thread
// also need extra thread for monitoring philos

// if only 1 philo?
//syncronize and join?

void *start_sim(t_simu *simu, t_monitor *mon)
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
		init_threads(simu, mon); // leak is here d^.^b
	return (NULL);
}
int main(int argc, char **argv)
{
	t_simu		simu;
	t_monitor	mon;

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

		start_sim(&simu, &mon); // leaking

		print_sim_values(&simu);




		free(simu.philosophers);
		free(simu.forks);
	}
	return (0);
}

