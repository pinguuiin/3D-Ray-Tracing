/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 20:48:54 by piyu              #+#    #+#             */
/*   Updated: 2025/09/24 00:24:20 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# ifndef WIDTH
#  define WIDTH 1024
# endif
# ifndef HEIGHT
#  define HEIGHT 768
# endif

# include "../src/libft/libft.h"
# include "../src/MLX42/include/MLX42/MLX42.h"
# include "vector.h"
# include <stdlib.h>
# include <math.h>

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
	double		viewport_rot[3];
	double		px;
	t_cam		cam;
	t_sphere	sphere;

}	t_info;

void	get_viewport_rotation(t_info *info, t_vec v1, t_vec v2);
void	rotate(t_vec *vec, double *theta);
void	draw(void *param);

#endif
