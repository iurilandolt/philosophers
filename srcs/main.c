/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 18:41:29 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/19 12:27:21 by rlandolt         ###   ########.fr       */
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
	init_threads(simu, mon);

	handle_mutex_op(&mon->mon_mtx, MTX_LOCK);
	simu->sim_rdy = true;
	simu->sim_srt = get_time_mls();
	handle_mutex_op(&mon->mon_mtx, MTX_UNLOCK);

	join_threads(simu, mon);

	printf("-> sim ready: %s @ %ld\n", simu->sim_rdy ? "true" : "false", simu->sim_srt);

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

