/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 20:48:54 by piyu              #+#    #+#             */
/*   Updated: 2025/09/20 01:52:19 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# ifndef WIDTH
#  define WIDTH 1536
# endif
# ifndef HEIGHT
#  define HEIGHT 1024
# endif

# include "../src/libft/libft.h"
# include "../src/MLX42/include/MLX42/MLX42.h"
#include <stdlib.h>
#include <math.h>

typedef struct s_color
{
}	t_color;

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
	double	fov; // converted to rad from degree  ==parsing==
	t_vec	pos;
	t_vec	direction;

}	t_cam;

/* Spheric object information */
typedef struct s_sphere
{
	t_vec	pos;
	t_vec	oc; // vector from object to camera
	double	r; // converted to radius from diameter  ==parsing==

}	t_sphere;

/* Coefficient a, b, c and discriminant delta for the quadratic equation */
typedef struct s_quad_coef
{
	double	a;
	double	b;
	double	c;
	double	delta;

}	t_quad_coef;

/* Struct that includes everything */
typedef struct s_info
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	double		focal_length;
	double		viewport_width;
	double		viewport_height;
	double		px;
	t_cam		cam;
	t_sphere	sphere;

}	t_info;

t_vec	vec3(double x, double y, double z);
double	norm(t_vec a);
t_vec	add(t_vec a, t_vec b);
t_vec	subtract(t_vec a, t_vec b);
double	dot(t_vec a, t_vec b);
t_vec	cross(t_vec a, t_vec b);
t_vec	divide(t_vec a, t_vec b);
void	rotate_viewport(t_vec *vec, t_vec v1, t_vec v2);

#endif
