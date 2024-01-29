/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 18:41:29 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/29 23:09:17 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	ft_error(char *str)
{
	printf("%s%s%s\n", R, str, RST);
}

void	exit_safe(t_sim *sim)
{
	if (sim)
		free(sim);
	ft_error("Invalid arguments.\n"
		G"Usage: qty.      | ms to die |"
		"ms to eat | ms to sleep | meal qty.\n"RST
		Y"       200[max]  | 60[min]   |"
		"60[min]   | 60[min]     | optional.\n"RST);
}

int	main(int argc, char **argv)
{
	t_sim	*sim;

	if (argc > 4 && argc < 7)
	{
		sim = (t_sim *)malloc(sizeof(t_sim));
		if (!sim)
			return (1);
		if (!parse(sim, argc, argv))
		{
			exit_safe(sim);
			return (1);
		}
		startup(sim);
		threads_create(sim);
		cleanup(sim);
		free(sim);
	}
	else
		exit_safe(NULL);
	return (0);
}
