/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 01:34:21 by piyu              #+#    #+#             */
/*   Updated: 2025/12/15 02:53:33 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

#ifndef BONUS

static inline void	get_hit_normal(t_info *info, t_object *obj, t_hit *hit)
{
	double	hit_h;

	if (obj->type == SPHERE)
		hit->normal = normalize(hit->op);
	else if (obj->type == CYLINDER)
	{
		hit_h = dot(hit->op, obj->axis);
		if (hit_h - obj->h > -EPSILON)
			hit->normal = obj->axis;
		else if (hit_h + obj->h < EPSILON)
			hit->normal = scale(obj->axis, -1);
		else
			hit->normal = normalize(subtract(hit->op,
						scale(obj->axis, hit_h)));
	}
	else if (obj->type == PLANE)
		hit->normal = obj->axis;
	if (dot(hit->normal, subtract(info->cam.pos, hit->pos)) < 0)
		hit->normal = scale(hit->normal, -1);
	hit->pos = add(hit->pos, scale(hit->normal, 0.0001));
}
#else

static inline void	get_hit_normal(t_object *obj, t_hit *hit)
{
	double	hit_h;

	if (obj->type == SPHERE && obj->normal)
	{
		sphere_tbn_to_xyz(obj, hit);
		return ;
	}
	if (obj->type == SPHERE)
		hit->normal = normalize(hit->op);
	else if (obj->type == CYLINDER)
	{
		hit_h = dot(hit->op, obj->axis);
		if (hit_h - obj->h > -EPSILON)
			hit->normal = obj->axis;
		else if (hit_h + obj->h < EPSILON)
			hit->normal = scale(obj->axis, -1);
		else
			hit->normal = normalize(subtract(hit->op,
						scale(obj->axis, hit_h)));
	}
	else if (obj->type == PLANE)
		hit->normal = obj->axis;
	if (dot(hit->normal, subtract(hit->emit_pos, hit->pos)) < 0)
		hit->normal = scale(hit->normal, -1);
	hit->pos = add(hit->pos, scale(hit->normal, 0.0001));
}
#endif

static inline void	add_diffuse_and_specular(t_hit *hit, t_light *light)
{
	double	flux;
	double	spec;

	flux = dot(hit->incoming, hit->normal);
	if (flux > EPSILON)
	{
		flux *= KD;
		hit->diffuse = scale(dot_elem(light->color, hit->color), flux);
		hit->intensity = add(hit->intensity, hit->diffuse);
		spec = dot(hit->outgoing, hit->ray);
		if (spec > EPSILON)
		{
			spec = KS * pow(spec, SHININESS);
			hit->specular = scale(light->color, spec);
			hit->intensity = add(hit->intensity, hit->specular);
		}
	}
}

/* Implement Phong reflection model:
Diffuse = Kd (incoming light · hit point surface normal)
=> Diffuse term is counted when its dot product is greater than 0
Specular = Ks (Reflected ray · ray to camera) ^ Shininess
=> Specular term is counted when both terms' dot products greater than 0;
Intensity = Diffuse + Specular (+ Ambient), and clamped to 0-255
*/
#ifndef BONUS

inline t_vec	reflection(t_info *info, t_object *obj, t_vec ray, t_hit *hit)
{
	t_light		*light;

	hit->intensity = dot_elem(info->amb, hit->color);
	hit->pos = add(info->cam_curr_frame.pos, ray);
	hit->ray = normalize(scale(ray, -1));
	hit->op = subtract(hit->pos, obj->pos);
	light = &info->light;
	hit->incoming = subtract(light->pos, hit->pos);
	hit->k_light = norm(hit->incoming);
	hit->incoming = normalize(hit->incoming);
	get_hit_normal(info, obj, hit);
	if (is_shadow(info, hit->incoming, hit->pos, hit))
		return (hit->intensity);
	hit->outgoing = scale(hit->normal, 2 * dot(hit->incoming, hit->normal));
	hit->outgoing = subtract(hit->outgoing, hit->incoming);
	add_diffuse_and_specular(hit, light);
	return (hit->intensity);
}
#else

inline t_vec	reflection(t_info *info, t_object *obj, t_vec ray, t_hit *hit)
{
	int			i;
	t_light		*light;

	i = -1;
	hit->intensity = dot_elem(info->amb, hit->color);
	hit->ray = normalize(scale(ray, -1));
	hit->op = subtract(hit->pos, obj->pos);
	while (i++ < info->n_light - 1)
	{
		light = &info->light[i];
		hit->incoming = subtract(light->pos, hit->pos);
		hit->k_light = norm(hit->incoming);
		hit->incoming = normalize(hit->incoming);
		if (i == 0)
			get_hit_normal(obj, hit);
		if (is_shadow(info, hit->incoming, hit->pos, hit))
			continue ;
		hit->outgoing = scale(hit->normal, 2 * dot(hit->incoming, hit->normal));
		hit->outgoing = subtract(hit->outgoing, hit->incoming);
		add_diffuse_and_specular(hit, light);
	}
	hit->bounce = scale(hit->normal, 2 * dot(hit->ray, hit->normal));
	hit->bounce = subtract(hit->bounce, hit->ray);
	return (hit->intensity);
}
#endif
