/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_tracer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 21:15:56 by ykadosh           #+#    #+#             */
/*   Updated: 2025/12/09 17:00:10 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline double	nearest_ray_hit(t_info *info, t_vec ray, t_hit *hit)
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

	k = nearest_ray_hit(info, ray, &hit);
	if (k == -1) // not hit
	{
		mlx_put_pixel(info->img, x, y, 0x000000FF);
		return ;
	}
	obj = &info->obj[hit.obj_id];
	hit.color = obj->color;
	color = dot_elem(info->amb, hit.color);
	if (!info->is_inside)
		color = add(color, reflection(info, obj, scale(ray, k), &hit));  // when camera on the object, k=0, the return will only include diffuse
	mlx_put_pixel(info->img, x, y, vec_to_color(color));
}
#else
// #include <stdio.h>
static inline void	get_texture_color_and_normal(t_hit *hit, t_object *obj)
{
	int	tex_loc[2];

	sphere_xyz_to_px_loc(hit->pos, obj, &tex_loc[0], &tex_loc[1]);
	hit->color = px_loc_to_color(obj->texture, tex_loc[0], tex_loc[1]);
	if (obj->normal)
		hit->normal = px_loc_to_normal(obj->normal, tex_loc[0], tex_loc[1]);
	// printf("x:%f  y:%f  z:%f\n",hit->normal.x, hit->normal.y, hit->normal.z);
}

static inline void	draw_pixel(t_info *info, t_vec ray, int x, int y)
{
	double		k;
	t_object	*obj;
	t_color		color;
	t_hit		hit;

	k = nearest_ray_hit(info, ray, &hit);
	if (k == -1) // not hit
	{
		mlx_put_pixel(info->img, x, y, 0x000000FF);
		return ;
	}
	obj = &info->obj[hit.obj_id];
	ray = scale(ray, k);
	hit.pos = add(info->cam_curr_frame.pos, ray);
	if (obj->type == SPHERE && obj->tex_file)
		get_texture_color_and_normal(&hit, obj);
	else
		hit.color = obj->color;
	color = dot_elem(info->amb, hit.color);
	if (!info->is_inside)
		color = add(color, reflection(info, obj, ray, &hit));  // when camera on the object, k=0, the return will only include diffuse
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
