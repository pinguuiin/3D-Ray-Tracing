/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multithreading.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:16:47 by ykadosh           #+#    #+#             */
/*   Updated: 2025/11/09 17:16:48 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void			*rendering_routine(void *ptr);
static inline void	draw_pixel(t_info *info, t_vec ray, int x, int y);
inline double		nearest_ray_hit(t_info *info, t_vec ray, t_hit *hit, t_object *obj);

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


void	init_threads(t_info *info)
{
	int	i;

	i = 0;
	while (i < N_THREADS)
	{
		if (pthread_create(&info->threads[i].painter, NULL, &rendering_routine, &info->threads[i]))
		{
			atomic_store(&info->exit_flag, 1);
			if (pthread_mutex_unlock(&info->render_lock)) // have to do this first as the created threads will want a piece of the lock first! Should not destroy the lock to early, while some thread might be trying to lock it.
			{
				// TODO: handle the error
			}
			let_threads_finish(info, i);
			unlock_mutex_if_locked_and_destroy(&info->render_lock, 0);
			exit (free_exit("Failed to create a thread; Aborting miniRT.", SYSTEM_FAILURE));
		}
		info->threads[i].p_info = info;
		info->threads[i].start_x = WIDTH / N_THREADS * i;
		if (N_THREADS && i < N_THREADS - 1) // first part is just to avoid overflow in the unlikely but theoretically possible case where N_THREADS would be defined to 1!
			info->threads[i].border_x = WIDTH / N_THREADS * (i + 1);
		else
			info->threads[i].border_x = WIDTH; // important, otherwise a pixel (or more) could be lost and not rendered, because of floating point truncations. We let the last thread do those until the very last one.

		i++;
	}
}


void	let_threads_finish(t_info *info, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		if (pthread_join(info->threads[j].painter, NULL))
			// TODO: at least write some error message....
		j++;
	}
}


// FIXME:
// - how do I organize the threads' routine, so that they know when a frame has ended? use a flag?
// - how is the renderer() going to quit if something fails / if ESC or the x button has been pressed by the user?
// - how does each thread know which thread they are from the array?
static void	*rendering_routine(void *ptr)
{
	t_painter	*painter;
	t_info		*info;
	t_vec		ray;
	int			x;
	int			y;

	painter = (t_painter *)ptr;
	info = painter->p_info;

	while (!atomic_load(&info->exit_flag))
	{
		if (pthread_mutex_lock(&info->render_lock))  // these lock/unlocks are probably not necessary!
		{
			// TODO: handle the error!

		}


		if (pthread_mutex_unlock(&info->render_lock))
		{
			// TODO: handle the error!

		}

		x = painter->start_x;
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

		atomic_fetch_add(&info->n_done_painters, 1);

	}
	return (NULL);
}

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

inline double	nearest_ray_hit(t_info *info, t_vec ray, t_hit *hit, t_object *obj)
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

// NOTE: this function should only be called just before exiting the program.
// Multithreading rendering in this program depends on the mutex, so if you
// destroy it, it means we are quiting anyways. Therefore, there is no need
// to add an extra quitting if something fails - but we can still acknowledge the
// failure by writing a message.
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
