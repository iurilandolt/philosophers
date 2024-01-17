/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 22:29:43 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/17 11:04:21 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/*
void philo_cycle(t_philo *philo)
{
	philo think - usleep N ms?
	pick up left fork - lock mutex?
	pick up right fork - lock mutex?
	philo eat - usleep N ms?
	put down left fork - unlock mutex?
	put down right fork - unlock mutex?
}
*/

static void	handle_mutex_error(int status)
{
	if (status != 0)
		ft_error("mutex operation error\n");
}

void	handle_mutex_op(pthread_mutex_t *mtx, t_mtxcode op)
{
	if (op == MTX_LOCK)
		handle_mutex_error(pthread_mutex_lock(mtx));
	else if (op == MTX_UNLOCK)
		handle_mutex_error(pthread_mutex_unlock(mtx));
	else if (op == MTX_INIT)
		handle_mutex_error(pthread_mutex_init(mtx, NULL));
	else if (op == MTX_DESTROY)
		handle_mutex_error(pthread_mutex_destroy(mtx));
}

static void	handle_thread_error(int status)
{
	if (status != 0)
		ft_error("thread operation error\n");
}

void	handle_thread_op(pthread_t	thread, void *(*func)(void *),
	void *data, t_thcode op)
{
	if (op == TH_CREATE)
		handle_thread_error(pthread_create(&thread, NULL, func, data));
	else if (op == TH_JOIN)
		handle_thread_error(pthread_join(thread, NULL));
	else if (op == TH_DETACH)
		handle_thread_error(pthread_detach(thread));
}
