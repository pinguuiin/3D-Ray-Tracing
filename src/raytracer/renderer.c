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

static inline double	nearest_ray_hit(t_info *info, t_vec ray, t_hit *hit, t_object *obj)

/*
* NOTE: this was copied to multithreading.c, since called from the routine.
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

* NOTE: this was copied to multithreading.c, since called from the routine.
static inline void	draw_pixel(t_info *info, t_vec ray, int x, int y)
{
	double		k;
	t_object	*obj;
	t_color		color;
	t_hit		hit;

	obj = NULL;
	k = nearest_ray_hit(info, ray, &hit, obj);
	if (k == -1.0) // not hit
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
*/


/*
 * // NOTE: this is the original function, for the mandatory part.
void	renderer(void *param)
{
	t_info		*info;
	int			x;
	int			y;
	t_vec		ray;


	info = (t_info *)param;
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
*/

/*
 * TODO: # ifdef MANDATORY ? use this original single threaded function, else
 * use the multithreaded one below...
 * // NOTE: this is the exact same function as the one for the mandatory part,
 * // but it is the fallback function for when multithreaded initiation/ or multi
 * // threads have failed! This way, the rendering can still with a single
 * // thread, despite the failure.
void	single_threaded_renderer(void *param)
{
	t_info		*info;
	int			x;
	int			y;
	t_vec		ray;


	info = (t_info *)param;
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
*/

//  TODO: # ifdef BONUS ???
//  FIXME: have to fix many things here!
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
