/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 18:41:29 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/20 00:46:30 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	ft_error(char *str) // pass t_simu, free allocated memory if any existss
{
	printf("%s%s%s\n", R, str, RST);
	exit(EXIT_FAILURE);
}

/*
if any of the arguments is zero
init_sim_values & validate_input() will stop the program from reacing this point
*/

void *start_sim(t_simu *simu, t_monitor *mon)
{
	simu->sim_rdy = false;
	simu->sim_end = false;
	simu->philosophers = alloc_philos(simu);
	simu->forks = alloc_forks(simu);
	init_monitor(simu, mon);
	all_threads_do(simu, mon, TH_CREATE);
	//handle_mutex_op(&mon->mon_mtx, MTX_LOCK);
	simu->sim_rdy = true;
	simu->sim_srt = get_time_mls();
	//handle_mutex_op(&mon->mon_mtx, MTX_UNLOCK);
	all_threads_do(simu, mon, TH_JOIN); // if we get here all philos are dead or full?
	set_bool(&mon->mon_mtx, &simu->sim_end, true);
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
		start_sim(&simu, &mon);

		print_sim_values(&simu);

		free(simu.philosophers);
		free(simu.forks);
	}
	return (0);
}

