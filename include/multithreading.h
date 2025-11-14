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

#ifndef MULTITHREADING_H
# define MULTITHREADING_H

# include <pthread.h>
# include <stdatomic.h>

# define N_THREADS 5

enum e_thread_status
{
	WAIT,
	RENDER,
	ABORT
};

typedef struct s_painter
{
	pthread_t		painter;
	struct s_info	*p_info;
	int				start_x;
	int				border_x;
}	t_painter;

typedef struct s_thread_system
{
	t_painter			threads[N_THREADS];
	pthread_barrier_t	barrier;
	atomic_int_fast32_t	status;
	atomic_int_fast32_t	n_done_painters;
	bool				is_multithreaded;
}	t_thread_system;

void	initialize_multithreading(struct s_info *info);
void	let_threads_finish(t_painter *threads, int i);
void	destruct_barrier(pthread_barrier_t *barrier);
void	multithreaded_renderer(void *param);

#endif
