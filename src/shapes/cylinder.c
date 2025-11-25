/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 02:01:58 by piyu              #+#    #+#             */
/*   Updated: 2025/11/25 20:41:13 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline double	hit_flat_disk(double is_upper_disk, double oc_n, double ray_n, double h)
{
	if (is_upper_disk > 0.0)
		oc_n -= h;
	else
		oc_n += h;
	return (-(oc_n / ray_n));
}

/* ray shooting from outside the infinite cylinder */
static inline double	hit_from_outside(t_object *cy, t_vec ray, t_discrim f, t_vec oc)
{
	double	hit_h[2];

	hit_h[0] = dot(add(oc, scale(ray, f.root)), cy->axis);
	if (fabs(hit_h[0]) - cy->h > EPSILON)  // closer intersection point P is out of boundary
	{
		f.root2 = (- f.b + sqrt(f.delta)) / (2 * f.a);
		hit_h[1] = dot(add(oc, scale(ray, f.root2)), cy->axis);
		if (hit_h[0] * hit_h[1] > EPSILON && fabs(hit_h[1]) - cy->h > EPSILON)  // ray is out of boundaries
			return (-1.0);
		return (hit_flat_disk(hit_h[0], f.oc_n, f.ray_n, cy->h));
	}
	return (f.root);
}

/* ray shooting from inside the infinite cylinder */
static inline double	hit_from_inside(t_object *cy, t_vec ray, t_discrim f, t_vec oc)
{
	double	hit_h;

	hit_h = dot(add(oc, scale(ray, f.root2)), cy->axis);
	if (fabs(f.oc_n) - cy->h < -EPSILON)  // inside the actual cylinder
	{
		get_info()->is_inside = true;
		if (fabs(hit_h) - cy->h < EPSILON)
			return (f.root2);
		return (hit_flat_disk(hit_h, f.oc_n, f.ray_n, cy->h));
	}
	if (fabs(f.oc_n) - cy->h <= EPSILON)  // camera on the object
		return (0.0);
	if (fabs(hit_h) - cy->h > EPSILON && hit_h * f.oc_n > EPSILON)  // ray out of boundary
		return (-1.0);
	return (hit_flat_disk(f.oc_n, f.oc_n, f.ray_n, cy->h));
}

static inline double	ray_parallel_axis(t_info *info, t_object *cy, t_discrim f)
{
	if (f.c > -EPSILON)  // shooting outside
		return (-1.0);
	if (fabs(f.oc_n) - cy->h < -EPSILON)  // camera inside the cylinder
	{
		info->is_inside = true;
		if (f.oc_n * f.ray_n < 0.0)
			return (cy->h + fabs(f.oc_n));
		return (cy->h - fabs(f.oc_n));
	}
	if (fabs(f.oc_n) - cy->h <= EPSILON)  // camera on the cylinder
		return (0.0);
	if (f.oc_n * f.ray_n < -EPSILON)  //cylinder in front of cam
		return (fabs(f.oc_n) - cy->h);
	return (-1.0);
}

inline double	ray_hit_cylinder(t_info *info, t_vec ray, t_object *cy, t_vec oc)
{
	t_discrim	f;

	f.oc_n = dot(oc, cy->axis);
	f.ray_n = dot(ray, cy->axis);
	f.a = 1.0 - f.ray_n * f.ray_n;
	f.b = 2 * (dot(oc, ray) - f.oc_n * f.ray_n);
	f.c = dot(oc, oc) - f.oc_n * f.oc_n - cy->r * cy->r;
	if (f.a < EPSILON)  // ray parallel with the cylinder axis
		return (ray_parallel_axis(info, cy, f));
	f.delta = f.b * f.b - 4.0 * f.a * f.c;
	if (f.delta >= EPSILON)  // delta = 0, ray is tangent to the cylinder, hit; root = 0, camera on the cylinder, ray hit
	{
		f.root = (- f.b - sqrt(f.delta)) / (2 * f.a);
		if (f.root > -EPSILON)  // ray hit from outside the infinitely long cylinder
			return (hit_from_outside(cy, ray, f, oc));
		f.root2 = (- f.b + sqrt(f.delta)) / (2 * f.a);
		if (f.root2 > -EPSILON)  // ray from inside the infinitely long cylinder
			return (hit_from_inside(cy, ray, f, oc));
	}
	return (-1.0);
}
