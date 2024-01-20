/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 18:42:03 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/20 13:10:19 by rlandolt         ###   ########.fr       */
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

int	init_sim_values(t_simu *simu, int argc, char **argv)
{
	if (!validate_arg(argv[1]) || !validate_arg(argv[2])
		|| !validate_arg(argv[3]) || !validate_arg(argv[4])
		|| !validate_arg(argv[5]))
		return (0);
	else
	{
		simu->seats = ft_atoi(argv[1]);
		simu->time_to_die = ft_atoi(argv[2]);
		simu->time_to_eat = ft_atoi(argv[3]);
		simu->time_to_sleep = ft_atoi(argv[4]);
		if (simu->seats < 1 || simu->time_to_die < 1
			|| simu->time_to_eat < 1 || simu->time_to_sleep < 1)
			return (0);
		if (argc == 6)
		{
			simu->meal_goal = ft_atoi(argv[5]);
			if (simu->meal_goal < 1)
				return (0);
		}
		else
			simu->meal_goal = -1;
	}
	return (1);
}
