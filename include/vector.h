/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 23:46:28 by piyu              #+#    #+#             */
/*   Updated: 2025/09/28 03:14:48 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

/* 3d vector coordinates */
typedef struct s_vec
{
	double	x;
	double	y;
	double	z;

}	t_vec;

t_vec	vec3(double x, double y, double z);
t_vec	scale(t_vec a, double k);
double	norm(t_vec a);
t_vec	normalize(t_vec a);
t_vec	add(t_vec a, t_vec b);
t_vec	subtract(t_vec a, t_vec b);
double	dot(t_vec a, t_vec b);
t_vec	cross(t_vec a, t_vec b);
t_vec	divide(t_vec a, t_vec b);
t_vec	dot_elem(t_vec a, t_vec b);

void	rotate_x(t_vec *vec, double theta);
void	rotate_y(t_vec *vec, double theta);
void	rotate_z(t_vec *vec, double theta);

#endif
