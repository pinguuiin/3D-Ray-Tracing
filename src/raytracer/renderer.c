/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 23:34:48 by piyu              #+#    #+#             */
/*   Updated: 2025/10/20 03:52:21 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	get_hit_attributes(t_info *info, t_vec ray, t_object *obj, t_hit *hit)
{
	double	hit_h;

	hit->pos = add(info->cam.pos, ray);
	hit->ray = normalize(scale(ray, -1));
	hit->op = subtract(hit->pos, obj->pos);
	hit->incoming = normalize(subtract(info->light.pos, hit->pos));
	if (obj->type == SPHERE)
		hit->normal = normalize(hit->op);
	else if (obj->type == CYLINDER)
	{
		hit_h = dot(hit->op, obj->normal);
		if (hit_h - obj->h > -EPSILON)
			hit->normal = obj->normal;
		else if (hit_h + obj->h < EPSILON)
			hit->normal = scale(obj->normal, -1);
		else
			hit->normal = normalize(subtract(hit->op, scale(obj->normal, hit_h)));
	}
	else if (obj->type == PLANE)
		hit->normal = obj->normal;
	hit->outgoing = scale(hit->normal, 2 * dot(hit->incoming, hit->normal));
	hit->outgoing = subtract(hit->outgoing, hit->incoming);
}

/* Implement Phong reflection model:
Diffuse = Kd (incoming light · object normal)
=> Diffuse term is counted when its dot product is greater than 0
Specular = Ks (Reflected ray · ray to camera) ^ Shininess
=> Specular term is counted when both terms' dot products greater than 0;
Intensity = Diffuse + Specular (+ Ambient), and clamped to 0-255
*/
t_vec	reflection(t_info *info, t_vec ray, t_hit *hit)
{
	t_object	*obj;
	double		flux;
	double		spec;

	obj = &info->obj[hit->obj_id];
	get_hit_attributes(info, ray, obj, hit);
	hit->intensity = vec3(0.0, 0.0, 0.0);
	flux = dot(hit->incoming, hit->normal);
	if (flux > EPSILON)
	{
		flux *= info->light.ratio * KD;
		hit->diffuse = scale(dot_elem(info->light.color, obj->color), flux);
		hit->intensity = add(hit->intensity, hit->diffuse);
		spec = dot(hit->outgoing, hit->ray);
		if (spec > EPSILON)
		{
			spec = info->light.ratio * KS * pow(spec, SHININESS);
			hit->specular = scale(info->light.color, spec);
			hit->intensity = add(hit->intensity, hit->specular);
		}
	}
	return (hit->intensity);
}

double	nearest_ray_hit(t_info *info, t_vec ray, t_hit *hit)
{
	double	k;
	double	k_min;
	int		id;

	k = -1.0;
	k_min = -1.0;
	id = 0;
	while (id < info->num)
	{
		if (info->obj[id].type == SPHERE)
			k = ray_hit_sphere(info, ray, id);
		else if (info->obj[id].type == PLANE)
			k = ray_hit_plane(info, ray, id);
		else
			k = ray_hit_cylinder(info, ray, id);
		if (k >= 0.0 && (k_min < -EPSILON || k < k_min))
		{
			k_min = k;
			hit->obj_id = id;
		}
		id++;
	}
	return (k_min);
}

void	draw_pixel(t_info *info, t_vec ray, int x, int y)
{
	double	k;
	t_vec	color;
	t_hit	hit;

	k = nearest_ray_hit(info, ray, &hit);
	if (k == -1) // not hit
	{
		mlx_put_pixel(info->img, x, y, vec_to_color(vec3(0.0, 0.0, 0.0)));
		return ;
	}
	color = scale(info->amb.color, info->amb.ratio);
	color = add(color, reflection(info, scale(ray, k), &hit));  // when camera on the object, k=0, the return will only include diffuse
	mlx_put_pixel(info->img, x, y, vec_to_color(color));
}

void	renderer(void *param)
{
	t_info		*info;
	int			x;
	int			y;
	t_vec		ray;


	info = (t_info *)param;
	x = 0;
	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
			ray = vec3(x * info->px - info->viewport_width / 2.0,
			-(y * info->px - info->viewport_height / 2.0), 0);
			rotate(info->rot, &ray);
			ray = add(info->cam.direction, ray);
			draw_pixel(info, ray, x, y);
			y++;
		}
		x++;
	}
}
