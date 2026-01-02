/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 02:01:58 by piyu              #+#    #+#             */
/*   Updated: 2026/01/02 04:32:39 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline double	hit_flat_disk(double is_upper_disk, double oc_n,
	double ray_n, double h)
{
	if (is_upper_disk > 0.0)
		oc_n -= h;
	else
		oc_n += h;
	return (-(oc_n / ray_n));
}

/* Ray hits from outside the infinite cylinder. Check if the first intersection
point P is out of finite boundary first, if not then return side hit root.
Otherwise, check the second intersection point, if out of boundary then no hit,
otherwise hit the flat disk on one end of the cylinder. */
static inline double	hit_from_outside(t_object *cy, t_vec ray, t_discrim f,
	t_vec oc)
{
	double	hit_h[2];

	hit_h[0] = dot(add(oc, scale(ray, f.root)), cy->axis);
	if (fabs(hit_h[0]) - cy->h > EPSILON)
	{
		f.root2 = (-f.b + sqrt(f.delta)) / (2 * f.a);
		hit_h[1] = dot(add(oc, scale(ray, f.root2)), cy->axis);
		if (hit_h[0] * hit_h[1] > EPSILON && fabs(hit_h[1]) - cy->h > EPSILON)
			return (-1.0);
		return (hit_flat_disk(hit_h[0], f.oc_n, f.ray_n, cy->h));
	}
	return (f.root);
}

/* Ray hits from inside the infinite cylinder. Check first if camera is inside
the finite cylinder, if so then check if it hits the side or one of the
flat disk. If camera is not inside then first check if camera is on the object.
If not, check if the hit point is out of boundary where ray doesn't hit. If not
then ray hits the flat disk from outside. */
static inline double	hit_from_inside(t_object *cy, t_vec o_side,
	t_discrim f)
{
	double	hit_h;

	hit_h = dot(o_side, cy->axis);
	if (fabs(f.oc_n) - cy->h < -EPSILON)
	{
		if (fabs(hit_h) - cy->h < EPSILON)
			return (f.root2);
		return (hit_flat_disk(hit_h, f.oc_n, f.ray_n, cy->h));
	}
	if (fabs(f.oc_n) - cy->h <= EPSILON)
		return (0.0);
	if (fabs(hit_h) - cy->h > EPSILON && hit_h * f.oc_n > EPSILON)
		return (-1.0);
	return (hit_flat_disk(f.oc_n, f.oc_n, f.ray_n, cy->h));
}

/* Camera is on the axis of the cylinder and ray is along the axis. Check first
if ray shoots towards outside. If so, ray doesn't hit. Then check if camera is
inside the cylinder. If yes then check which disk it hits. If camera is not
inside then check if it's on the cylinder. If not then camera is shooting
towards the cylinder from outside, calculate the distance. */
static inline double	ray_parallel_axis(t_object *cy, t_discrim f)
{
	if (f.c > -EPSILON)
		return (-1.0);
	if (fabs(f.oc_n) - cy->h < -EPSILON)
	{
		if (f.oc_n * f.ray_n < 0.0)
			return (cy->h + fabs(f.oc_n));
		return (cy->h - fabs(f.oc_n));
	}
	if (fabs(f.oc_n) - cy->h <= EPSILON)
		return (0.0);
	if (f.oc_n * f.ray_n < -EPSILON)
		return (fabs(f.oc_n) - cy->h);
	return (-1.0);
}

/* Ray hitting cylinder check. First check if ray is on the axis which may
result in divided-by-0 case. Then use discriminant to select the conditions
when there are one or two intersection points with the infinite cylinder.
If the smaller root is greater than 0, infinite cylinder is in front of the
camera. Otherwise check the larger root, if it's greater than 0 then camera
is inside the infinite cylinder, otherwise cylinder is behind the camera,
ray doesn't hit*/
inline double	ray_hit_cylinder(t_vec ray, t_object *cy, t_vec oc)
{
	t_discrim	f;

	f.oc_n = dot(oc, cy->axis);
	f.ray_n = dot(ray, cy->axis);
	f.a = 1.0 - f.ray_n * f.ray_n;
	f.b = 2 * (dot(oc, ray) - f.oc_n * f.ray_n);
	f.c = dot(oc, oc) - f.oc_n * f.oc_n - cy->r * cy->r;
	if (f.a < EPSILON)
		return (ray_parallel_axis(cy, f));
	f.delta = f.b * f.b - 4.0 * f.a * f.c;
	if (f.delta >= EPSILON)
	{
		f.root = (-f.b - sqrt(f.delta)) / (2 * f.a);
		if (f.root > -EPSILON)
			return (hit_from_outside(cy, ray, f, oc));
		f.root2 = (-f.b + sqrt(f.delta)) / (2 * f.a);
		if (f.root2 > -EPSILON)
			return (hit_from_inside(cy, add(oc, scale(ray, f.root2)), f));
	}
	return (-1.0);
}
