/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 20:48:54 by piyu              #+#    #+#             */
/*   Updated: 2025/09/17 22:56:13 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

#include <math.h>

/* 3d vector coordinates */
typedef struct s_vec
{
	double	x;
	double	y;
	double	z;

}	t_vec;

/* camera information */
typedef struct s_cam
{
	int		fov;
	t_vec	pos;
	t_vec	normal;

}	t_cam;

/* Spheric object information */
typedef struct s_sphere
{
	t_vec	pos;
	t_vec	r;

}	t_sphere;

/* Struct that includes everything */
typedef struct s_info
{
	t_cam		cam;
	t_sphere	sphere;

}	t_info;

t_vec	add(t_vec a, t_vec b);
t_vec	subtract(t_vec a, t_vec b);
t_vec	dot(t_vec a, t_vec b);
t_vec	cross(t_vec a, t_vec b);
t_vec	divide(t_vec a, t_vec b);

#endif
