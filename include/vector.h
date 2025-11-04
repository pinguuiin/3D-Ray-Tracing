/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 23:46:28 by piyu              #+#    #+#             */
/*   Updated: 2025/10/20 02:28:19 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

/* 3d vector coordinates */
typedef union u_vec
{
	struct
	{
		double	x;
		double	y;
		double	z;
	};

	struct
	{
		double	r;
		double	g;
		double	b;
	};
}	t_vec;

typedef t_vec	t_color;

t_vec	vec3(double x, double y, double z);
t_vec	scale(t_vec a, double k);
double	norm(t_vec a);
t_vec	normalize(t_vec a);
t_vec	add(t_vec a, t_vec b);
t_vec	subtract(t_vec a, t_vec b);
double	dot(t_vec a, t_vec b);
t_vec	cross(t_vec a, t_vec b);
// t_vec	divide(t_vec a, t_vec b);
t_vec	dot_elem(t_vec a, t_vec b);

#endif
