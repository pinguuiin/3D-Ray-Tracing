/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 18:04:16 by ykadosh           #+#    #+#             */
/*   Updated: 2025/11/18 18:35:04 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

#ifndef BONUS
#else
static void	let_threads_finish(t_painter *threads, int i);
static void	destruct_barrier(pthread_barrier_t *barrier);

void	clean_up_threads_and_barrier(t_thread_system *thread_system, int i)
{
	let_threads_finish(thread_system->threads, i);
	destruct_barrier(&thread_system->barrier);
}

static void	let_threads_finish(t_painter *threads, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		if (pthread_join(threads[j].painter, NULL))
			ft_putstr_fd("Failed to join one of the threads.\n", 2);
		j++;
	}
}

static void	destruct_barrier(pthread_barrier_t *barrier)
{
	if (pthread_barrier_destroy(barrier))
		ft_putstr_fd("Failed to deallocate barrier used for multithreading "
			"purposes.\n", 2);
}
#endif
