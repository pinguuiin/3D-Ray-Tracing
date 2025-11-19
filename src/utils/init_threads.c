/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 18:03:58 by ykadosh           #+#    #+#             */
/*   Updated: 2025/11/18 18:35:04 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "minirt.h"

#ifndef BONUS
#else
static int	init_barrier(t_thread_system *thread_system);
static void	init_threads(t_info *info);
/*
* If the barrier's initialization fails, this program falls back on single
* threaded execution.
*/
void	initialize_multithreading(struct s_info *info)
{
	if (N_THREADS <= 0)
		return ;
	if (init_barrier(&info->thread_system) == -1)
		atomic_store(&info->thread_system.is_multithreaded, 0);
	else
	{
		atomic_store(&info->thread_system.is_multithreaded, 1);
		init_threads(info);
	}
}

/*
* Initializes the barrier for the threads. As soon as a thread waits on that
* barrier - by calling pthread_barrier_wait() - it blocks all threads (including
* the main thread) waiting on it, up until the moment where the last of them
* starts waiting on it. At that point, the barrier is 'removed' and all threads
* continue their routine.
*/
static int	init_barrier(t_thread_system *thread_system)
{
	if (pthread_barrier_init(&thread_system->barrier, NULL, N_THREADS + 1))
	{
		ft_putstr_fd("Failed to initialize barrier for multithreaded execution;"
			" initiating fallback to single-threaded rendering.\n", 2);
		return (-1);
	}
	return (0);
}

/*
* Creates all threads.
*
* In case of failure:
* Sets 'is_multithreaded' to false, communicating to the rendering hook that a
* fallback to single-threaded execution is necessasry.
* If that failure occurs during the creation loop, after at least one thread is
* already up and running, those threads should not wait on the barrier; if they
* do, the barrier would never detect sufficient threads waiting on it, since it
* expects the amount of threads that was passed to it in init_barrier(). The
* waiting threads would therefore get stuck there indefinitely. This is why the
* flag 'is_done_init' exists, and is only set at the end of init_threads(),
* after all expected threads have been created. As soon as the threads
* start their routine, they enter a while loop which checks that flag as well as
* the 'exit_flag', preventing them from the undesired wait at the barrer.
*
* In case of success:
* This function assigns to each thread the left and right borders of the chunk
* of the frame that they are supposed to render (in this implementation, the
* whole frame is split into equal vertical slices).
*/
static void	init_threads(t_info *info)
{
	t_painter	*thread;
	int			i;

	i = 0;
	while (i < N_THREADS)
	{
		thread = &info->thread_system.threads[i];
		thread->p_info = info;
		init_chunk_borders(info->img->width, thread, i);
		if (pthread_create(&thread->painter, NULL, &rendering_routine, thread))
		{
			if (i)
				atomic_store(&info->thread_system.exit_flag, 1);
			ft_putstr_fd("Failed to create a thread; initiating fallback to "
				"single-threaded rendering.\n", 2);
			clean_up_threads_and_barrier(&info->thread_system, i);
			atomic_store(&info->thread_system.is_multithreaded, 0); // signals that we fallback to single-threaded rendering.
			return ;
		}
		i++;
	}
	atomic_store(&info->thread_system.is_done_init, 1);
}

inline void	init_chunk_borders(int32_t width, t_painter *thread, int i)
{
	thread->start_x = width / N_THREADS * i;
	if (i == N_THREADS - 1)
		thread->border_x = width;
	else
		thread->border_x = width / N_THREADS * (i + 1);
}
#endif
