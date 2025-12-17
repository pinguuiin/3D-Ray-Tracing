/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 02:05:48 by piyu              #+#    #+#             */
/*   Updated: 2025/12/12 01:46:21 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
/* delta = 0 means ray tangent to the sphere, ray hits.
root = 0 means camera on the sphere, ray hits with distance = 0 */
inline double	ray_hit_sphere(t_vec ray, t_object *sphere, t_vec oc)
{
	t_discrim	f;

	f.a = 1.0;
	f.b = 2 * dot(ray, oc);
	f.c = dot(oc, oc) - sphere->r * sphere->r;
	f.delta = f.b * f.b - 4.0 * f.a * f.c;
	if (f.delta >= EPSILON)
	{
		f.root = (-f.b - sqrt(f.delta)) / (2 * f.a);
		if (f.root >= EPSILON)
			return (f.root);
		f.root2 = (-f.b + sqrt(f.delta)) / (2 * f.a);
		if (f.root2 >= EPSILON)
			return (f.root2);
	}
	return (-1.0);
}
