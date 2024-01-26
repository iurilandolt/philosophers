/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 18:41:29 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/26 13:51:29 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
#include <string.h>

void	exit_safe(t_sim *sim)
{
	if (sim)
		free(sim);
	ft_error("Invalid arguments.\n"
		G"Usage: qty.      | ms to die | ms to eat | ms to sleep | meal qty.\n"RST
		Y"       200[max]  | 60[min]   | 60[min]   | 60[min]     | optional.\n"RST);
}

int	main(int argc, char **argv)
{
	t_sim	*sim;

	if (argc > 4 && argc < 7)
	{
		sim = (t_sim *)malloc(sizeof(t_sim));
		if (!sim)
			ft_error("malloc() failed.");
		if (!parse(sim, argc, argv))
			exit_safe(sim);
		startup(sim);
		threads_create(sim);
		//print_sim_values(sim);
		cleanup(sim);
		free(sim);
	}
	else
		exit_safe(NULL);
	return (0);
}
