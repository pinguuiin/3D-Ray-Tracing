/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 22:15:24 by piyu              #+#    #+#             */
/*   Updated: 2025/09/26 00:53:00 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec	vec3(double x, double y, double z)
{
	t_vec	v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}

t_vec	scale(t_vec a, double k)
{
	t_vec	v;

	v.x = k * a.x;
	v.y = k * a.y;
	v.z = k * a.z;
	return (v);
}

double	norm(t_vec a)
{
	return (sqrt(a.x * a.x + a.y * a.y + a.z * a.z));
}

t_vec	normalize(t_vec a)
{
	if (norm(a) == 0)
		return (vec3(0.0, 0.0, 0.0));
	return (scale(a, 1.0 / norm(a)));
}

t_vec	add(t_vec a, t_vec b)
{
	t_vec	v;

	v.x = a.x + b.x;
	v.y = a.y + b.y;
	v.z = a.z + b.z;
	return (v);
}

t_vec	subtract(t_vec a, t_vec b)
{
	t_vec	v;

	v.x = a.x - b.x;
	v.y = a.y - b.y;
	v.z = a.z - b.z;
	return (v);
}

double	dot(t_vec a, t_vec b)
{
	double	product;

	product = a.x * b.x + a.y * b.y + a.z * b.z;
	return (product);
}

t_vec	cross(t_vec a, t_vec b)
{
	t_vec	v;

	v.x = a.y * b.z - a.z * b.y;
	v.y = a.z * b.x - a.x * b.z;
	v.z = a.x * b.y - a.y * b.x;
	return (v);
}

t_vec	divide(t_vec a, t_vec b)
{
	t_vec	v;

	// need extra action when divided by 0
	v.x = a.x / b.x;
	v.y = a.y / b.y;
	v.z = a.z / b.z;
	return (v);
}

t_vec	dot_elem(t_vec a, t_vec b)
{
	t_vec	v;

	v.x = a.x * b.x;
	v.y = a.y * b.y;
	v.z = a.z * b.z;
	return (v);
}
