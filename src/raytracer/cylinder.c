/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 02:01:58 by piyu              #+#    #+#             */
/*   Updated: 2025/10/21 01:03:12 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	hit_from_outside(t_object *cy, t_vec ray, t_discrim f)
{
	double		hit_h[2];

	hit_h[0] = dot(add(cy->oc, scale(ray, f.root)), cy->normal);
	if (fabs(hit_h[0]) - cy->h > EPSILON)  // closer intersection point P is out of boundary
	{
		f.root2 = (- f.b + sqrt(f.delta)) / (2 * f.a);
		hit_h[1] = dot(add(cy->oc, scale(ray, f.root2)), cy->normal);
		if (hit_h[0] * hit_h[1] > EPSILON && fabs(hit_h[1]) - cy->h > EPSILON)  // ray is out of boundaries
			return (-1.0);
		if (hit_h[0] > 0)
			f.oc_n -= cy->h;
		else
			f.oc_n += cy->h;
		f.root = -(f.oc_n / f.ray_n);
		return (f.root);
	}
	return (f.root);
}

double	hit_from_inside(t_info *info, t_object *cy, t_vec ray, t_discrim f)
{
	double		hit_h;

	hit_h = dot(add(cy->oc, scale(ray, f.root2)), cy->normal);
	if (fabs(hit_h) - cy->h > EPSILON && hit_h * f.oc_n >= -EPSILON)// intersection point P is out of boundary
		return (-1.0);
	if (fabs(f.oc_n) - cy->h < -EPSILON)  // inside
	{
		info->is_inside = true;
		if (fabs(hit_h) - cy->h > EPSILON)
		{

		}
	}
	if (fabs(f.oc_n) - cy->h <= EPSILON)  // camera on the object
		return (0.0);
	if (f.oc_n > 0)
		f.oc_n -= cy->h;
	else
		f.oc_n += cy->h;
	f.root = -(f.oc_n / f.ray_n);
	return (f.root);
}

double	ray_hit_cylinder(t_info *info, t_vec ray, int id)
{
	t_object	*cy;
	t_discrim	f;

	cy = &info->obj[id];
	f.oc_n = dot(cy->oc, cy->normal);
	f.ray_n = dot(ray, cy->normal);
	f.a = dot(ray, ray) - f.ray_n * f.ray_n;
	f.b = 2 * (dot(cy->oc, ray) - f.oc_n * f.ray_n);
	f.c = dot(cy->oc, cy->oc) - f.oc_n * f.oc_n - cy->r * cy->r;
	if (f.a < EPSILON)  // ray on the same direction with the cylinder axis, ray hit
	{
		if (f.c > -EPSILON)  // shooting outside
			return (-1.0);
		if (fabs(f.oc_n) - cy->h < -EPSILON)  // camera inside the cylinder
		{
			info->is_inside = true;
			if (f.oc_n * f.ray_n < 0.0)
				return ((cy->h + fabs(f.oc_n)) / f.ray_n);
			return ((cy->h - fabs(f.oc_n)) / f.ray_n);
		}
		if (fabs(f.oc_n) - cy->h <= EPSILON)  // camera on the cylinder
			return (0.0);
		if (f.oc_n * f.ray_n < -EPSILON)  //object center in front of cam; inside cylinder is partially here!
			return ((fabs(f.oc_n) - cy->h) / f.ray_n);
		return (-1.0);
	}
	f.delta = f.b * f.b - 4.0 * f.a * f.c;
	if (f.delta >= EPSILON)  // delta = 0, ray is tangent to the cylinder, hit; root = 0, camera on the cylinder, ray hit
	{
		f.root = (- f.b - sqrt(f.delta)) / (2 * f.a);
		if (f.root >= EPSILON)  // ray hit from outside the infinitely long cylinder
			return (hit_from_outside(cy, ray, f));
		f.root2 = (- f.b + sqrt(f.delta)) / (2 * f.a);
		if (f.root2 >= EPSILON)  // ray from inside the infinitely long cylinder
			return (hit_from_inside(info, cy, ray, f));
		// else inside the cylinder or cylinder behind camera
	}
	return (-1.0);
}
