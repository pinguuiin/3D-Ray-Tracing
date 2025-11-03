/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 01:34:21 by piyu              #+#    #+#             */
/*   Updated: 2025/11/03 03:05:46 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool	is_shadow(t_info *info, t_vec ray, t_vec pos, t_hit *hit)
{
	int		id;
	double	k;

	id = 0;
	while (id < info->n_obj)
	{
		if (id == hit->obj_id)
		{
			id++;
			continue ;
		}
		if (info->obj[id].type == SPHERE)
			k = ray_hit_sphere(info, ray, &info->obj[id], pos);
		else if (info->obj[id].type == PLANE)
			k = ray_hit_plane(ray, &info->obj[id], pos);
		else
			k = ray_hit_cylinder(info, ray, &info->obj[id], pos);
		if (k > EPSILON)
			return (true);
		id++;
	}
	return (false);
}

void	get_hit_normal(t_object *obj, t_hit *hit)
{
	double	hit_h;

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
}

int	get_hit_attributes(t_info *info, t_object *obj, t_vec ray, t_hit *hit)
{
	hit->pos = add(info->cam.pos, ray);
	hit->ray = normalize(scale(ray, -1));
	hit->op = subtract(hit->pos, obj->pos);
	hit->incoming = normalize(subtract(info->light.pos, hit->pos));
	if (is_shadow(info, hit->incoming, hit->pos, hit))
		return (1);
	get_hit_normal(obj, hit);
	hit->outgoing = scale(hit->normal, 2 * dot(hit->incoming, hit->normal));
	hit->outgoing = subtract(hit->outgoing, hit->incoming);
	return (0);
}

/* Implement Phong reflection model:
Diffuse = Kd (incoming light · object normal)
=> Diffuse term is counted when its dot product is greater than 0
Specular = Ks (Reflected ray · ray to camera) ^ Shininess
=> Specular term is counted when both terms' dot products greater than 0;
Intensity = Diffuse + Specular (+ Ambient), and clamped to 0-255
*/
t_vec	reflection(t_info *info, t_object *obj, t_vec ray, t_hit *hit)
{
	double	flux;
	double	spec;

	hit->intensity = vec3(0.0, 0.0, 0.0);
	if (get_hit_attributes(info, obj, ray, hit))
		return (hit->intensity);
	flux = dot(hit->incoming, hit->normal);
	if (flux > EPSILON)
	{
		flux *= KD;
		hit->diffuse = scale(dot_elem(info->light.color, obj->color), flux);
		hit->intensity = add(hit->intensity, hit->diffuse);
		spec = dot(hit->outgoing, hit->ray);
		if (spec > EPSILON)
		{
			spec = KS * pow(spec, SHININESS);
			hit->specular = scale(info->light.color, spec);
			hit->intensity = add(hit->intensity, hit->specular);
		}
	}
	return (hit->intensity);
}
