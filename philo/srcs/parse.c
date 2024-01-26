/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 18:42:03 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/24 14:01:24 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	else
		return (0);
}

static int	validate_arg(char *str)
{
	int	i;

	if (!str)
		return (1);
	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		if (i > 10)
			return (0);
		i++;
	}
	return (1);
}

int	ft_atoi(char *str)
{
	int	n;
	int	sign;

	n = 0;
	sign = 1;
	while (*str == 32)
		str++;
	if (*str == '+' || *str == '-')
		if (*str++ == '-')
			sign = -1;
	while (*str >= '0' && *str <= '9')
	{
		n = n * 10 + (*str++ - '0');
		if (n > INT_MAX)
			return (0);
	}
	return (n * sign);
}

int	check_boundaries(t_sim *sim)
{
	if (sim->seats < 1 || sim->seats > 200)
		return (0);
	if (sim->time_to_die < 60)
		return (0);
	if (sim->time_to_eat < 60)
		return (0);
	if (sim->time_to_sleep < 60)
		return (0);
	return (1);
}

int	parse(t_sim *sim, int argc, char **argv)
{
	if (!validate_arg(argv[1]) || !validate_arg(argv[2])
		|| !validate_arg(argv[3]) || !validate_arg(argv[4])
		|| !validate_arg(argv[5]))
		return (0);
	else
	{
		sim->seats = ft_atoi(argv[1]);
		sim->time_to_die = ft_atoi(argv[2]);
		sim->time_to_eat = ft_atoi(argv[3]);
		sim->time_to_sleep = ft_atoi(argv[4]);
		if (!check_boundaries(sim))
			return (0);
		if (argc == 6)
		{
			sim->goal = ft_atoi(argv[5]);
			if (sim->goal < 1)
				return (0);
		}
		else
			sim->goal = -1;
	}
	return (1);
}
