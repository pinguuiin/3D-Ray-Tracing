/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 23:34:48 by piyu              #+#    #+#             */
/*   Updated: 2025/09/27 03:52:07 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec	reflection(t_info *info, t_object *obj, t_vec ray, double k)
{
	t_hit	hit;

	if (k <= 0)
		return (vec3(0.0, 0.0, 0.0));
	ray = scale(ray, k);
	hit.pos = add(info->cam.pos, ray);
	hit.incoming = normalize(subtract(info->light.pos, hit.pos));
	if (obj->type == SPHERE)
		hit.normal = normalize(subtract(hit.pos, obj->pos));
	else if (obj->type == PLANE)
	{
		if (dot(ray, obj->normal) < 0)
			hit.normal = obj->normal;
		else
			hit.normal = scale(obj->normal, -1);
	}
	hit.outgoing = subtract(scale(hit.normal, 2 * dot(hit.incoming, hit.normal)), hit.incoming);
	hit.ray = normalize(scale(ray, -1));
	hit.diffuse = scale(dot_elem(info->light.color, obj->color), info->light.ratio * KD * dot(hit.incoming, hit.normal));
	hit.specular = scale(info->light.color, info->light.ratio * KS * pow(dot(hit.outgoing, hit.ray), SHININESS));
	hit.intensity = add(hit.diffuse, hit.specular);
	return (hit.intensity);
}

void	draw_sphere(t_info *info, t_vec ray, int x, int y)
{
	t_object	*sphere;
	t_discrim	f;
	t_vec		color;

	sphere = &info->obj[info->obj_id];
	f.a = dot(ray, ray);
	f.b = 2 * dot(ray, sphere->oc);
	f.c = dot(sphere->oc, sphere->oc) - sphere->r * sphere->r;
	f.delta = f.b * f.b - 4.0 * f.a * f.c;
	color = scale(info->amb.color, info->amb.ratio);
	if (f.delta >= 0)
		color = add(color, reflection(info, sphere, ray, (- f.b - sqrt(f.delta)) / (2 * f.a))); //now ambient also inside object
	mlx_put_pixel(info->img, x, y, vec_to_color(color));
}

void	draw_plane(t_info *info, t_vec ray, int x, int y)
{
	t_object	*plane;
	t_discrim	f;
	t_vec		color;
	
	plane = &info->obj[info->obj_id];
	f.a = dot(plane->oc, plane->normal);
	f.b = dot(ray, plane->normal);
	color = scale(info->amb.color, info->amb.ratio);
	if (f.b != 0 && -(f.a / f.b) > 0)
		color = add(color, reflection(info, plane, ray, -(f.a / f.b))); 
	mlx_put_pixel(info->img, x, y, vec_to_color(color));

}

void	draw_pixel(t_info *info, t_vec ray, int x, int y)
{
	if (info->obj[info->obj_id].type == SPHERE)
		draw_sphere(info, ray, x, y);
	else
		draw_plane(info, ray, x, y);
}

void	draw(void *param)
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
			rotate(&ray, info->viewport_rot);
			ray = add(info->cam.direction, ray);
			draw_pixel(info, ray, x, y);
			y++;
		}
		x++;
	}
}
