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

static inline void		render_column(int x, t_info *info);
static inline void		draw_pixel(t_info *info, t_vec ray, int x, int y);
static inline double	nearest_ray_hit(t_info *info, t_vec ray, t_hit *hit, t_object *obj);
static inline void		update_data_for_new_frame(t_info *info);


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

#ifndef BONUS
static inline void	update_data_for_new_frame(t_info *info)
{
	if (info->has_moved)
	{
		info->has_moved = 0;
		update_oc_and_plane_normal(info);
	}
	if ((uint32_t) info->mlx->height != info->img->height
		|| (uint32_t)info->mlx->width != info->img->width)
		resize(info->mlx->width, info->mlx->height, info);
	info->is_inside = false;
}
#else
static inline void	update_data_for_new_frame(t_info *info)
{
	int	i;

	if (info->has_moved)
	{
		info->has_moved = 0;
		update_oc_and_plane_normal(info);
	}
	if ((uint32_t) info->mlx->height != info->img->height
		|| (uint32_t)info->mlx->width != info->img->width)
	{
		resize(info->mlx->width, info->mlx->height, info);
		if (atomic_load(&info->thread_system.is_multithreaded))
		{
			i = 0;
			while (i < N_THREADS)
			{
				init_chunk_borders(info->img->width,
					&info->thread_system.threads[i], i);
				i++;
			}
		}
	}
	info->is_inside = false;
}
#endif

static inline void	render_column(int x, t_info *info)
{
	t_vec		ray;
	uint32_t	y;

	y = 0;
	while (y < info->img->height)
	{
		ray = vec3(x * info->px - info->viewport_w / 2.0,
		-(y * info->px - info->viewport_h / 2.0), 0);
		rotate(info->rot, &ray);
		ray = normalize(add(info->cam.direction, ray));
		draw_pixel(info, ray, x, y);
		y++;
	}
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
		// NOTE: we change the vec_to_color() call to black, for optimization :-)
		mlx_put_pixel(info->img, x, y, 0x000000FF);
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
