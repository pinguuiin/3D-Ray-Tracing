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

enum e_routine_action
{
	WAIT,
	RENDER,
	ABORT
};

enum e_mt_status
{
	MT_OFF,
	MT_ON,
	MT_FAILURE
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
	atomic_int_fast32_t	routine_action;
	atomic_int_fast32_t	n_done_painters;
	atomic_int_fast32_t	mt_status;
}	t_thread_system;

void	initialize_multithreading(struct s_info *info);
void	renderer(void *param);
void	clean_up_threads_and_barrier(t_thread_system *thread_system, int i);

# endif
#endif
