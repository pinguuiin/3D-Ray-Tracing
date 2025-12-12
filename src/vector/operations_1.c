/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 02:01:27 by piyu              #+#    #+#             */
/*   Updated: 2025/12/12 02:03:18 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

inline t_vec	vec3(double x, double y, double z)
{
	t_vec	v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}

inline t_vec	scale(t_vec a, double k)
{
	t_vec	v;

	v.x = k * a.x;
	v.y = k * a.y;
	v.z = k * a.z;
	return (v);
}

inline double	norm(t_vec a)
{
	return (sqrt(a.x * a.x + a.y * a.y + a.z * a.z));
}

inline t_vec	normalize(t_vec a)
{
	double	len;

	len = norm(a);
	if (len < EPSILON)
		return (vec3(0.0, 0.0, 0.0));
	return (scale(a, 1.0 / len));
}
