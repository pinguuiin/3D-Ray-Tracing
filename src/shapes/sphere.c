/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 02:05:48 by piyu              #+#    #+#             */
/*   Updated: 2025/10/22 02:45:05 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	ray_hit_sphere(t_info *info, t_vec ray, t_object *sphere, t_vec pos)
{
	t_vec		oc;
	t_discrim	f;

	oc = subtract(pos, sphere->pos);
	f.a = 1.0;
	f.b = 2 * dot(ray, oc);
	f.c = dot(oc, oc) - sphere->r * sphere->r;
	f.delta = f.b * f.b - 4.0 * f.a * f.c;
	if (f.delta >= EPSILON) // delta = 0, ray is tangent to the sphere, hit; root = 0, camera on the sphere, ray hit
	{
		f.root = (- f.b - sqrt(f.delta)) / (2 * f.a);
		if (f.root >= EPSILON)
			return (f.root);
		f.root2 = (- f.b + sqrt(f.delta)) / (2 * f.a);
		if (f.root2 >= EPSILON)
		{
			info->is_inside = true;
			return (f.root2);
		}
	}
	return (-1.0);
}
