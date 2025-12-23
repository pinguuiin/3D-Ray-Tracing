/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_tracer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 21:15:56 by ykadosh           #+#    #+#             */
/*   Updated: 2025/12/23 06:41:46 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

inline double	nearest_ray_hit(t_info *info, t_vec ray, t_vec emit_pos, t_hit *hit)
{
	int			id;
	double		k;
	double		k_min;
	t_object	*obj;

	id = 0;
	k = -1.0;
	k_min = -1.0;
	while (id < info->n_obj)
	{
		obj = &info->obj[id];
		if (obj->type == SPHERE)
			k = ray_hit_sphere(ray, obj, subtract(emit_pos, obj->pos));
		else if (obj->type == PLANE)
			k = ray_hit_plane(ray, obj, subtract(emit_pos, obj->pos));
		else
			k = ray_hit_cylinder(ray, obj, subtract(emit_pos, obj->pos));
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
/* Color pixel to black when ray doesn't hit.
k = 0 means camera on the object, ray hits */
static inline t_color	trace_ray(t_vec ray, t_hit *hit, int depth, t_color overlay)
{
	double		k;
	t_info		*info;
	t_object	*obj;
	t_color		color;

	(void)depth;
	(void)overlay;
	info = get_info();
	k = nearest_ray_hit(info, ray, info->cam_curr_frame.pos, hit);
	if (k + 1.0 <= EPSILON)
		return (vec3(0.0, 0.0, 0.0));
	obj = &info->obj[hit->obj_id];
	hit->color = obj->color;
	color = reflection(info, obj, scale(ray, k), hit);
	return (color);
}

#else

static inline void	get_texture_color_and_normal(t_hit *hit, t_object *obj)
{
	int	tex_loc[2];

	if (obj->type == SPHERE)
		sphere_xyz_to_px_loc(hit->pos, obj, &tex_loc[0], &tex_loc[1]);
	else
		plane_xyz_to_px_loc(hit->pos, obj, &tex_loc[0], &tex_loc[1]);
	hit->color = px_loc_to_color(obj, tex_loc[0], tex_loc[1]);
	if (obj->normal)
		hit->normal = px_loc_to_normal(obj->normal, tex_loc[0], tex_loc[1]);
}

static inline t_color	trace_ray(t_vec ray, t_hit *hit, int depth, t_color overlay)
{
	double		k;
	t_info		*info;
	t_color		color;
	t_object	*obj;

	info = get_info();
	if (depth <= 0)
		return (vec3(0.0, 0.0, 0.0));
	k = nearest_ray_hit(info, ray, hit->emit_pos, hit);
	if (k + 1.0 <= EPSILON)
		return (vec3(0.0, 0.0, 0.0)) ;
	obj = &info->obj[hit->obj_id];
	ray = scale(ray, k);
	hit->pos = add(hit->emit_pos, ray);
	hit->f = obj->ks;
	if (obj->material != MONO)
		get_texture_color_and_normal(hit, obj);
	else
		hit->color = obj->color;
	color = dot_elem(overlay, reflection(info, obj, ray, hit));
	overlay = scale(dot_elem(overlay, hit->color), hit->f);
	ray = normalize(hit->bounce);
	hit->emit_pos = hit->pos;
	color = add(color, trace_ray(ray, hit, depth - 1, overlay));
	return (color);
}
#endif

inline void	render_column(int x, t_info *info)
{
	uint32_t	y;
	t_vec		ray;
	t_color		color;
	t_hit		hit;

	y = 0;
	while (y < info->img->height)
	{
		ray = vec3(x * info->px - info->viewport_w / 2.0,
				-(y * info->px - info->viewport_h / 2.0), 0);
		rotate(info->rot, &ray);
		ray = normalize(add(info->cam_curr_frame.direction, ray));
		hit.emit_pos = info->cam_curr_frame.pos;
		color = trace_ray(ray, &hit, info->ray_depth, vec3(1.0, 1.0, 1.0));
		mlx_put_pixel(info->img, x, y, vec_to_color(color));
		y++;
	}
}
