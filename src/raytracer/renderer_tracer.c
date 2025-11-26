/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_tracer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 21:15:56 by ykadosh           #+#    #+#             */
/*   Updated: 2025/11/26 04:11:00 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

inline void	update_camera_for_new_frame(t_info *info)
{
	if (info->has_rotated)
	{
		info->has_rotated = 0;
		info->cam_curr_frame.direction = info->cam.direction;
		get_rotation_matrix(info, info->cam_curr_frame.direction);
	}
	if (info->has_moved)
	{
		info->has_moved = 0;
		info->cam_curr_frame.pos = info->cam.pos;
		update_oc_and_plane_normal(info);
	}
}

static inline double	nearest_ray_hit(t_info *info, t_vec ray, t_hit *hit,
							t_object *obj)
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

#ifndef BONUS
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
#else
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
	color = dot_elem(info->amb, obj->color);  // need fixxxxxxxxxxxxxxx
	if (!info->is_inside)
		color = add(color, reflection(info, obj, scale(ray, k), &hit));  // when camera on the object, k=0, the return will only include diffuse
	mlx_put_pixel(info->img, x, y, vec_to_color(color));
}
#endif

inline void	render_column(int x, t_info *info)
{
	t_vec		ray;
	uint32_t	y;

	y = 0;
	while (y < info->img->height)
	{
		ray = vec3(x * info->px - info->viewport_w / 2.0,
		-(y * info->px - info->viewport_h / 2.0), 0);
		rotate(info->rot, &ray);
		ray = normalize(add(info->cam_curr_frame.direction, ray));
		draw_pixel(info, ray, x, y);
		y++;
	}
}
