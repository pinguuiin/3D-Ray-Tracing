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

void	renderer(void *param)
{
	t_info		info;
	t_vec		ray;
	int			n_tasks_done;
	int			i;
	p_thread_t
	// int		x;
	// int		y;


	info = (t_info *)param;
	info->is_inside = false;

	

	init_threads(info);

	// tell the threads to start rendering (first time.)
	info->should_render = 1;


	// WARN: need to ask someone about this:
	// should I rather use a mutex, so that I am certain of a controlled framework
	// for my threads' synchornicity? Or would that impede on the efficiency and synchornicity?

	// Allow the threads enough time to notice that should_render is 1,
	// but not too much so that they (or some of them) would end their assigned chunk of the frame
	// before should_render is reset to 0 ????
	usleep(20);
	info->should_render = 0;

	n_tasks_done = 0;
	i = 0
	while (n_tasks_done < N_THREADS)
	{
		while (i < N_THREADS)
		{
			if (info->threads[i].is_done)
				n_tasks_done++;
			i++;
		}
		i = 0;
	}

	// TODO: check that all threads are done (by combining their "is_done" variables?)

	/*
	x = 0;
	while (x < info->img->width)
	{
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
		x++;
	}
	*/

	// TODO: add a joining of threads function.
}
