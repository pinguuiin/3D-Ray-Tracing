/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 20:48:54 by piyu              #+#    #+#             */
/*   Updated: 2025/09/26 03:52:23 by piyu             ###   ########.fr       */
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
# ifndef KS
#  define KS 0.4
# endif
# ifndef KD
#  define KD 0.2
# endif
# ifndef SHININESS
#  define SHININESS 30
# endif

# include "../src/libft/libft.h"
# include "../src/MLX42/include/MLX42/MLX42.h"
# include "vector.h"
# include <stdlib.h>
# include <math.h>

/* Type of object */
typedef enum e_type
{
	SPHERE,
	PLANE,
	CYLINDER

}	t_type;

/* Ambient light */
typedef struct s_ambient
{
	t_vec	color;  // each rgb channel normalized from 0-255 to 0.0-1.0 and saved into xyz  ==parsing==
	double	ratio;

}	t_ambient;

/* Key light */
typedef struct s_light
{
	t_vec	pos;
	t_vec	color;
	double	ratio;

}	t_light;

/* Camera specs */
typedef struct s_cam
{
	double	fov; // converted to rad from degree  ==parsing==
	t_vec	pos;
	t_vec	direction;

}	t_cam;

/* Spheric object information */
typedef struct s_sphere
{
	t_type	type;
	t_vec	pos;
	t_vec	oc; // vector from object to camera
	double	r; // converted to radius from diameter  ==parsing==
	t_vec	color; // rgba

}	t_sphere;

/* Cylindrical object information */
typedef struct s_cylinder
{
	t_type	type;
	t_vec	pos;
	t_vec	oc;
	t_vec	normal;
	double	r;
	double	h;
	t_vec	color;

}	t_cylinder;

/* Plane object information */
typedef struct s_plane
{
	t_type	type;
	t_vec	pos;
	t_vec	normal;
	t_vec	color;

}	t_plane;

/* Coefficient a, b, c and discriminant delta for the quadratic equation */
typedef struct s_quad_coef
{
	double	a;
	double	b;
	double	c;
	double	delta;

}	t_quad_coef;

typedef struct s_hit
{
	t_vec	pos;  // coordinates of the hit point
	t_vec	incoming;  // normalized ray direction from hit point to light source
	t_vec	normal;  // normal at the hit point
	t_vec	ray;  // normalized ray direction from hit point to camera
	t_vec	outgoing; // normalized specular reflected ray direction
	t_vec	diffuse;  // diffuse reflection intensity
	t_vec	specular;  // specular reflection intensity
	t_vec	intensity;  // intensity = diffuse + specular

}	t_hit;

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
	t_ambient	amb;
	t_light		light;
	void		*obj;  // array of objects; saved as void pointer so that it can contain different structs
	void		*obj_lst;  // list of objects; will be cleared up after parsing

}	t_info;

void	get_viewport_rotation(t_info *info, t_vec v1, t_vec v2);
void	rotate(t_vec *vec, double *theta);
void	draw(void *param);

#endif
