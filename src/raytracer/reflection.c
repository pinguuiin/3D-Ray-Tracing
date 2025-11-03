/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 01:34:21 by piyu              #+#    #+#             */
/*   Updated: 2025/11/03 09:36:45 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool	is_shadow(t_info *info, t_vec ray, t_vec pos, t_hit *hit)
{
	int			id;
	double		k;
	t_object	*obj;

	id = 0;
	while (id < info->n_obj)
	{
		if (id == hit->obj_id)
		{
			id++;
			continue ;
		}
		obj = &info->obj[id];
		if (obj->type == SPHERE)
			k = ray_hit_sphere(info, ray, obj, subtract(pos, obj->pos));
		else if (obj->type == PLANE)
			k = ray_hit_plane(ray, obj, subtract(pos, obj->pos));
		else
			k = ray_hit_cylinder(info, ray, obj, subtract(pos, obj->pos));
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
			hit->normal = normalize(subtract(hit->op,
				scale(obj->normal, hit_h)));
	}
	else if (obj->type == PLANE)
		hit->normal = obj->normal;
}

void	add_diffuse_and_specular(t_reflect *ref, t_hit *hit,
		t_light *light, t_object *obj)
{
	double	flux;
	double	spec;

	flux = dot(ref->incoming, hit->normal);
	if (flux > EPSILON)
	{
		flux *= KD;
		ref->diffuse = scale(dot_elem(light->color, obj->color), flux);
		hit->intensity = add(hit->intensity, ref->diffuse);
		spec = dot(ref->outgoing, hit->ray);
		if (spec > EPSILON)
		{
			spec = KS * pow(spec, SHININESS);
			ref->specular = scale(light->color, spec);
			hit->intensity = add(hit->intensity, ref->specular);
		}
	}
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
	int			i;
	t_reflect	ref;
	t_light		*light;

	i = 0;
	hit->intensity = vec3(0.0, 0.0, 0.0);
	hit->pos = add(info->cam.pos, ray);
	hit->ray = normalize(scale(ray, -1));
	hit->op = subtract(hit->pos, obj->pos);
	while (i < info->n_light)
	{
		light = &info->light[i];
		ref.incoming = normalize(subtract(light->pos, hit->pos));
		if (is_shadow(info, ref.incoming, hit->pos, hit))
		{
			i++;
			continue ;
		}
		get_hit_normal(obj, hit);
		ref.outgoing = scale(hit->normal, 2 * dot(ref.incoming, hit->normal));
		ref.outgoing = subtract(ref.outgoing, ref.incoming);
		add_diffuse_and_specular(&ref, hit, light, obj);
		i++;
	}
	return (hit->intensity);
}
