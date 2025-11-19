/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multithreading.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:01:20 by ykadosh           #+#    #+#             */
/*   Updated: 2025/11/13 23:58:31 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BONUS
#else
# ifndef MULTITHREADING_H
#  define MULTITHREADING_H

# include <pthread.h>
# include <stdatomic.h>

# define N_THREADS 5

/*
* NOTE: uint32_t is used for the 'x' axis values to avoid casting, because they
* are often compared against the 'mlx->img->width' value, which is of that same
* data type as well.
*/
typedef struct s_painter
{
	pthread_t		painter;
	struct s_info	*p_info;
	uint32_t		start_x;
	uint32_t		border_x;
}	t_painter;

typedef struct s_thread_system
{
	t_painter						threads[N_THREADS];
	pthread_barrier_t				barrier;
	volatile atomic_bool			is_done_init;
	volatile atomic_bool			is_multithreaded;
	volatile atomic_bool			exit_flag;
	volatile atomic_int_fast32_t	n_done_painters;
}	t_thread_system;

void	initialize_multithreading(struct s_info *info);
void	init_chunk_borders(int32_t width, t_painter *thread, int i);
void	renderer(void *param);
void	*rendering_routine(void *ptr);
void	clean_up_threads_and_barrier(t_thread_system *thread_system, int i);

# endif
#endif
