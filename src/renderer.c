/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 23:34:48 by piyu              #+#    #+#             */
/*   Updated: 2025/10/03 23:52:57 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/* Implement Phong reflection model:
Diffuse = Kd (incoming light · object normal)
=> Diffuse term is counted when its dot product is greater than 0
Specular = Ks (Reflected ray · ray to camera) ^ Shininess
=> Specular term is counted when both terms' dot products greater than 0;
Intensity = Diffuse + Specular (+ Ambient), and clamped to 0-255
*/
t_vec	reflection(t_info *info, t_object *obj, t_vec ray, double k)
{
	t_hit	hit;
	double	dot1;
	double	dot2;

	ray = scale(ray, k);
	hit.pos = add(info->cam.pos, ray);
	hit.ray = normalize(scale(ray, -1));
	hit.incoming = normalize(subtract(info->light.pos, hit.pos));
	if (obj->type == SPHERE)
		hit.normal = normalize(subtract(hit.pos, obj->pos));
	else if (obj->type == PLANE)
		hit.normal = obj->normal;
	hit.outgoing = subtract(scale(hit.normal, 2 * dot(hit.incoming, hit.normal)), hit.incoming);
	hit.intensity = vec3(0.0, 0.0, 0.0);
	dot1 = dot(hit.incoming, hit.normal);
	if (dot1 > EPSILON)
	{
		hit.diffuse = scale(dot_elem(info->light.color, obj->color), info->light.ratio * KD * dot1);
		hit.intensity = add(hit.intensity, hit.diffuse);
		dot2 = dot(hit.outgoing, hit.ray);
		if (dot2 > EPSILON)
		{
			hit.specular = scale(info->light.color, info->light.ratio * KS * pow(dot2, SHININESS));
			hit.intensity = add(hit.intensity, hit.specular);
		}
	}
	return (hit.intensity);
}

double	ray_hit_sphere(t_info *info, t_vec ray)
{
	t_object	*sphere;
	t_discrim	f;

	sphere = &info->obj[info->obj_id];
	f.a = dot(ray, ray);
	f.b = 2 * dot(ray, sphere->oc);
	f.c = dot(sphere->oc, sphere->oc) - sphere->r * sphere->r;
	f.delta = f.b * f.b - 4.0 * f.a * f.c;
	if (f.delta >= EPSILON) // delta = 0, ray is tangent to the sphere, hit; root = 0, camera on the sphere, ray hit
		return ((- f.b - sqrt(f.delta)) / (2 * f.a));
	return (-1.0);
}

double	ray_hit_plane(t_info *info, t_vec ray)
{
	t_object	*plane;
	t_discrim	f;

	plane = &info->obj[info->obj_id];
	f.a = dot(plane->oc, plane->normal);
	f.b = dot(ray, plane->normal);
	f.delta = fabs(f.b);
	if (f.delta > EPSILON) // delta = 0, ray parallel to the plane, not hit; root = 0, camera on the plane, ray hit
		return (-(f.a / f.b));
	return (-1.0);
}

double	nearest_ray_hit(t_info *info, t_vec ray)
{
	double	k;
	double	k_min;
	int		nearest_id;

	k = -1;
	k_min = -1;
	nearest_id = 0;
	info->obj_id = 0;
	while (info->obj_id < info->num)
	{
		if (info->obj[info->obj_id].type == PLANE)
		// 	k = ray_hit_sphere(info, ray);
		// else
			k = ray_hit_plane(info, ray);
		if (k > 0.0)
		{
			if (info->obj_id == 1 || k < k_min)
			{
				k_min = k;
				nearest_id = info->obj_id;
			}
		}
		info->obj_id++;
	}
	info->obj_id = nearest_id;
	return (k_min);
}

void	draw_pixel(t_info *info, t_vec ray, int x, int y)
{
	double	k;
	t_vec	color;

	k = nearest_ray_hit(info, ray);
	if (k == -1)
	{
		mlx_put_pixel(info->img, x, y, vec_to_color(vec3(0.0, 0.0, 0.0)));
		return ;
	}
	color = scale(info->amb.color, info->amb.ratio);
	color = add(color, reflection(info, &info->obj[info->obj_id], ray, k));
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
