/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 02:06:23 by piyu              #+#    #+#             */
/*   Updated: 2025/11/03 05:33:47 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/* root = -(f.a / f.b)
f.a = 0, camera on the plane, ray hit, k=0;
f.a != 0, f.b = 0, ray parallel to the plane, not hit
f.a != 0, f.b != 0, root > 0, ray hit
*/
double	ray_hit_plane(t_vec ray, t_object *plane, t_vec oc)
{
	t_discrim	f;

	f.a = dot(oc, plane->normal);
	f.b = dot(ray, plane->normal);
	if (fabs(f.a) < EPSILON)
		return (0.0);
	if (fabs(f.b) > EPSILON)
	{
		f.root = -(f.a / f.b);
		if (f.root > EPSILON)
			return (f.root);
	}
	return (-1.0);
}
