/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 22:29:43 by rlandolt          #+#    #+#             */
/*   Updated: 2024/01/18 18:51:53 by rlandolt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
#include <errno.h>

static void	handle_mutex_error(int status, t_mtxcode op)
{
	if (!status)
		return ;
	if (status == EINVAL && (op == MTX_LOCK || op == MTX_UNLOCK))
		ft_error("The value specified by mutex is invalid");
	else if (status == EINVAL && op == MTX_INIT)
		ft_error("The value specified by attr is invalid");
	else if (status == EDEADLK)
		ft_error("A deadlock condition was detected");
	else if (status == EPERM)
		ft_error("The current thread does not hold a lock on mutex.");
	else if (status == ENOMEM)
		ft_error("The process cannot allocate enough memory");
	else if (status == EBUSY)
		ft_error("Mutex is locked.");
}

void	handle_mutex_op(pthread_mutex_t *mtx, t_mtxcode op)
{
	if (op == MTX_LOCK)
		handle_mutex_error(pthread_mutex_lock(mtx), op);
	else if (op == MTX_UNLOCK)
		handle_mutex_error(pthread_mutex_unlock(mtx), op);
	else if (op == MTX_INIT)
		handle_mutex_error(pthread_mutex_init(mtx, NULL), op);
	else if (op == MTX_DESTROY)
		handle_mutex_error(pthread_mutex_destroy(mtx), op);
}

static void	handle_thread_error(int status, t_thcode op)
{
	if (!status)
		return ;
	if (status == EAGAIN)
		ft_error("Not enough resources to create another thread");
	else if (status == EPERM)
		ft_error("No appropriate permission to create the thread");
	else if (status == EINVAL && op == TH_CREATE)
		ft_error("The value specified by attr is invalid.");
	else if (status == EINVAL && (op == TH_JOIN || op == TH_DETACH))
		ft_error("The value specified by thread is not joinable/detachable.");
	else if (status == ESRCH)
		ft_error("No correspondent thread to given thread ID");
	else if (status == EDEADLK)
		ft_error("A deadlock condition was detected");
}

void	handle_thread_op(pthread_t	*thread, void *(*func)(void *),
	void *data, t_thcode op)
{
	if (op == TH_CREATE)
		handle_thread_error(pthread_create(thread, NULL, func, data), op);
	else if (op == TH_JOIN)
		handle_thread_error(pthread_join(*thread, NULL), op);
	else if (op == TH_DETACH)
		handle_thread_error(pthread_detach(*thread), op);
}
