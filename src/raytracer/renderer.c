/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 23:34:48 by piyu              #+#    #+#             */
/*   Updated: 2025/11/26 22:08:42 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

#ifndef BONUS
#else
/*
* Since pthread_barrier_wait() can only fail if the barrier is not initialized
* beforehand, this program does no error handling for that failure: The barrier
* is initialized in the init_barrier() function, and if that initialization does
* not succeed, no thread is created and rendering happens a single process.
*/
inline void	*rendering_routine(void *ptr)
{
	t_painter	*painter;
	t_info		*info;
	uint32_t	x;

	painter = (t_painter *)ptr;
	info = painter->p_info;
	while (!atomic_load(&info->thread_system.is_done_init))
	{
		(void)usleep(1000);
		if (atomic_load(&info->thread_system.exit_flag))
			return (NULL);
	}
	while (1)
	{
		x = painter->start_x;
		(void)pthread_barrier_wait(&info->thread_system.barrier);
		if (atomic_load(&info->thread_system.exit_flag))
			return (NULL);
		while (x < painter->border_x)
			render_column(x++, info);
		atomic_fetch_add(&info->thread_system.n_done_painters, 1);
	}
	return (NULL);
}
#endif

#ifndef BONUS
void	renderer(void *param)
{
	t_info		*info;
	uint32_t	x;

	info = (t_info *)param;
	update_data_for_new_frame(info);
	x = 0;
	while (x < info->img->width)
		render_column(x++, info);
}
#else
void	renderer(void *param)
{
	t_info			*info;
	t_thread_system	*thread_system;
	uint32_t		x;

	info = (t_info *)param;
	update_data_for_new_frame(info);
	thread_system = &info->thread_system;
	if (!atomic_load(&thread_system->is_multithreaded))
	{
		// fallback to single_threaded rendering!
		x = 0;
		while (x < info->img->width)
			render_column(x++, info);
		return ;
	}
	pthread_barrier_wait(&thread_system->barrier);
	while (atomic_load(&thread_system->n_done_painters) < N_THREADS)
		(void)usleep(200);
	atomic_store(&thread_system->n_done_painters, 0);
}
#endif
