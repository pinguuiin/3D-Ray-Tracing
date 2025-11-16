/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 23:34:48 by piyu              #+#    #+#             */
/*   Updated: 2025/11/18 23:16:12 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

#ifndef BONUS
#else
static void			init_barrier(t_thread_system *thread_system);
static void			init_threads(t_thread_system *thread_system);
static inline void	*rendering_routine(void *ptr);
#endif
static inline void		draw_pixel(t_info *info, t_vec ray, int x, int y);
static inline double	nearest_ray_hit(t_info *info, t_vec ray, t_hit *hit, t_object *obj);

/*
 * WARN: is this even used??? - mutex implementation
void	init_and_lock_mutual_exclusion_object(t_info *info)
{
	if (pthread_mutex_init(&info->render_lock, NULL))
		exit (free_exit("Failed to initialize mutual exclusion object, crucial "
			"for the threads' safe synchronicity.", SYSTEM_FAILURE));
	if (pthread_mutex_lock(&info->render_lock))
	{
		unlock_mutex_if_locked_and_destroy(&info->render_lock, 0);
		exit (free_exit("Failed to lock mutual exclusion object, crucial "
			"for the threads' safe synchronicity.", SYSTEM_FAILURE));
	}
}
*/

/*
// NOTE: this function should only be called just before exiting the program.
// Multithreading rendering in this program depends on the mutex, so if you
// destroy it, it means we are quiting anyways. Therefore, there is no need
// to add an extra quitting if something fails - but we can still acknowledge the
// failure by writing a message.
// WARN: is this even used? - mutex implementation
void	unlock_mutex_if_locked_and_destroy(pthread_mutex_t *render_lock,
			bool is_locked)
{
	if (is_locked)
	{
		if (pthread_mutex_unlock(render_lock))
		{
			// TODO: handle the error. Just write a message, do not quit, let it destroy first?
			
		
		}
	}
	if (pthread_mutex_destroy(render_lock))
	{
		// TODO: handle the error, simply by writing some message.
		// this call was cast to (void) in another project you worked on.

	}
}
*/

#ifndef BONUS
#else
void	initialize_multithreading(struct s_info *info)
{
	init_barrier(&info->thread_system);
	if (info->thread_system.is_multithreaded)
	{
		info->thread_system.status = WAIT;
		info->thread_system.threads[0].p_info = info;
		init_threads(&info->thread_system);
	}
}

static void	init_barrier(t_thread_system *thread_system)
{
	if (pthread_barrier_init(&thread_system->barrier, NULL, N_THREADS))
	{
		ft_putstr_fd("Failed to initialize barrier for multithreaded execution;"
			" initiating fallback to single-threaded rendering.\n",
			2);
		thread_system->is_multithreaded = 0;
	}
	else
		thread_system->is_multithreaded = 1;
}

static void	init_threads(t_thread_system *thread_system)
{
	int				i;

	i = 0;
	while (i < N_THREADS)
	{
		if (pthread_create(&thread_system->threads[i].painter, NULL,
			&rendering_routine, &thread_system->threads[i]))
		{
			atomic_store(&thread_system->status, ABORT); // signals to the other threads that they should return.
			let_threads_finish(thread_system->threads, i);
			ft_putstr_fd("Failed to create a thread; initiating fallback to "
				"single-threaded rendering.\n", 2);
			destruct_barrier(&thread_system->barrier);
			thread_system->is_multithreaded = 0; // signals that we fallback to the single threaded rendering function, since multithreading has failed.
			return ;
		}
		thread_system->threads[i].p_info = thread_system->threads[0].p_info;
		thread_system->threads[i].start_x = WIDTH / N_THREADS * i;
		if (N_THREADS && i < N_THREADS - 1) // first part is just to avoid overflow in the unlikely but theoretically possible case where N_THREADS would be defined to 1!
			thread_system->threads[i].border_x = WIDTH / N_THREADS * (i + 1);
		else
			thread_system->threads[i].border_x = WIDTH; // important, otherwise a pixel (or more) could be lost and not rendered, because of floating point truncations. We let the last thread do those until the very last one.

		i++;
	}
}

void	let_threads_finish(t_painter *threads, int i)
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

void	destruct_barrier(pthread_barrier_t *barrier)
{
	if (pthread_barrier_destroy(barrier))
		ft_putstr_fd("Failed to deallocate barrier used for multithreading "
			"purposes.\n", 2);
}

static inline void	*rendering_routine(void *ptr)
{
	t_painter	*painter;
	t_info		*info;
	t_vec		ray;
	int			x;
	int			y;
	int			retval;
	// The purpose of 'retval' is to catch the return value of
	// pthread_barrier_wait() because that function can return:
	//	- 0 (if successful)
	//	- the macro PTHREAD_BARRIER_SERIAL_THREAD (if successful, for one of the
	//	- threads synchronised at the barrier)
	//	- an error value in case of an error.

	painter = (t_painter *)ptr;
	info = painter->p_info;

	while (atomic_load(&info->thread_system.status) != ABORT)
	{
		x = painter->start_x;
		while (atomic_load(&info->thread_system.status) == WAIT
			&& atomic_load(&info->thread_system.n_done_painters))
		{
			if (usleep(500))
			{
				// TODO : handle the error!!!!
				// fallback to other single-threaded version?
			}
		}
		retval = pthread_barrier_wait(&info->thread_system.barrier);
		if (retval && retval != PTHREAD_BARRIER_SERIAL_THREAD)
		{
			ft_putstr_fd("A thread has failed to wait for the barrier which "
				"ensures synchronisation with other threads;\n"
				"Initiating fallback to single-threaded rendering.\n", 2);
			// WARN: is this message finally appropriate? Are we falling back finally?
			// TODO:
			// 1. Fallback to single-threaded renderer - but how??? We already
			// assigned one function to our loop_hook...
			// 2. If NOT falling back, review the comment above.
		}
		while (x < painter->border_x)
		{
			y = 0;
			while (y < HEIGHT)
			{
				ray = vec3(x * info->px - info->viewport_width / 2.0,
				-(y * info->px - info->viewport_height / 2.0), 0);
				rotate(info->rot, &ray);
				ray = normalize(add(info->cam.direction, ray));
				draw_pixel(info, ray, x, y);
				y++;
			}
			x++;
		}

		atomic_fetch_add(&info->thread_system.n_done_painters, 1);

	}
	// this extra barrier_wait call is added here to handle a scenario where:
	// one fast thread would load a false value for exit_flag at the start of
	// the previous while loop, and go on waiting at the barrier that follows -
	// while a slower thread would, in the meanwhile, notice that exit_flag is
	// set, and exit the loop - which would create a deadlock at the barrier!
	retval = pthread_barrier_wait(&info->thread_system.barrier);
	if (retval && retval != PTHREAD_BARRIER_SERIAL_THREAD)
	{
		ft_putstr_fd("A thread has failed to wait for the barrier which "
			"ensures synchronisation with other threads\n", 2);
	}
	return (NULL);
}
#endif

/*
 * FIXME: How do I organize the mandatory vs. bonus function here, knowing
 * that I might want to use the 'mandatory' function as a fallback if BONUS is
 * defined but is_multithreaded is 0, because of some failure in the threads?
 * NOTE: this is the original renderer() function, from before the introduction
 * of multithreading into this project.
*/
#ifndef BONUS
void	single_threaded_renderer(void *param)
{
	t_info		*info;
	int			x;
	int			y;
	t_vec		ray;

	info = (t_info *)param;
	if (info->exit_flag)
	{
		mlx_close_window(info->mlx);
		return ;
	}
	info->is_inside = false;
	x = 0;
	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
			ray = vec3(x * info->px - info->viewport_width / 2.0,
			-(y * info->px - info->viewport_height / 2.0), 0);
			rotate(info->rot, &ray);
			ray = normalize(add(info->cam.direction, ray));
			draw_pixel(info, ray, x, y);
			y++;
		}
		x++;
	}
}
#else
//  FIXME:
//  - design function's fallback on single threaded rendering.
//  - solve tearing happening on each of the threads' start of chunks when moving camera
//  - issues with closing the window with the ESC button
//  - other issues with window's 'x' button
void	multithreaded_renderer(void *param)
{
	t_info			*info;
	t_thread_system	*thread_system;

	info = (t_info *)param;
	thread_system = &info->thread_system;
	info->is_inside = false;

	if (atomic_load(&thread_system->status) == ABORT)
	{
		let_threads_finish(thread_system->threads, N_THREADS);
		destruct_barrier(&thread_system->barrier);
		mlx_close_window(info->mlx);
		return ;
	}

	atomic_store(&thread_system->status, RENDER);


	// WARN: is this good enough? Should I sleep more? or is it even too much?
	// maybe this is the origin for some tests that suddenly froze?
	// All threads should:
	// 	- notice the new RENDER flag
	// 	- go on to wait at the barrier (until all threads are there)
	// 	- start rendering, finish rendering their chunk
	// 	- by which time, the status flag HAS TO BE AGAIN == WAIT!!
	if (usleep(400) == -1)
	{
		// TODO: handle error?
	}
	if (atomic_load(&thread_system->status) != ABORT)
		atomic_store(&thread_system->status, WAIT);

	while (atomic_load(&thread_system->n_done_painters) < N_THREADS)
	{
		if (usleep(200) == -1)
		{
			// TODO: handle the error!

		}
	}
	atomic_store(&thread_system->n_done_painters, 0);

}
#endif

static inline void	draw_pixel(t_info *info, t_vec ray, int x, int y)
{
	double		k;
	t_object	*obj;
	t_color		color;
	t_hit		hit;

	obj = NULL;
	k = nearest_ray_hit(info, ray, &hit, obj);
	if (k == -1) // not hit
	{
		mlx_put_pixel(info->img, x, y, vec_to_color(vec3(0.0, 0.0, 0.0)));
		return ;
	}
	obj = &info->obj[hit.obj_id];
	color = dot_elem(info->amb, obj->color);
	if (!info->is_inside)
		color = add(color, reflection(info, obj, scale(ray, k), &hit));  // when camera on the object, k=0, the return will only include diffuse
	mlx_put_pixel(info->img, x, y, vec_to_color(color));
}

static inline double	nearest_ray_hit(t_info *info, t_vec ray, t_hit *hit, t_object *obj)
{
	double	k;
	double	k_min;
	int		id;

	k = -1.0;
	k_min = -1.0;
	id = 0;
	while (id < info->n_obj)
	{
		obj = &info->obj[id];
		if (obj->type == SPHERE)
			k = ray_hit_sphere(info, ray, obj, obj->oc);
		else if (obj->type == PLANE)
			k = ray_hit_plane(ray, obj, obj->oc);
		else
			k = ray_hit_cylinder(info, ray, obj, obj->oc);
		if (k >= 0.0 && (k_min < -EPSILON || k < k_min))
		{
			k_min = k;
			hit->obj_id = id;
		}
		id++;
	}
	return (k_min);
}
