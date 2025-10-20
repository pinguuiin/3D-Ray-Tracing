/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 02:05:48 by piyu              #+#    #+#             */
/*   Updated: 2025/10/20 02:05:58 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	ray_hit_sphere(t_info *info, t_vec ray, int id)
{
	t_object	*sphere;
	t_discrim	f;

	sphere = &info->obj[id];
	f.a = dot(ray, ray);
	f.b = 2 * dot(ray, sphere->oc);
	f.c = dot(sphere->oc, sphere->oc) - sphere->r * sphere->r;
	f.delta = f.b * f.b - 4.0 * f.a * f.c;
	if (f.delta >= EPSILON) // delta = 0, ray is tangent to the sphere, hit; root = 0, camera on the sphere, ray hit
	{
		f.root = (- f.b - sqrt(f.delta)) / (2 * f.a);
		if (f.root >= EPSILON)
			return (f.root);
		// else inside the sphere or sphere behind camera
	}
	return (-1.0);
}