/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 20:48:54 by piyu              #+#    #+#             */
/*   Updated: 2025/10/01 23:37:34 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# ifndef WIDTH
#  define WIDTH 1500
# endif
# ifndef HEIGHT
#  define HEIGHT 1000
# endif
# ifndef EPSILON
#  define EPSILON 1e-8
# endif
# ifndef KS
#  define KS 0.5
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

/* Object information */
typedef struct s_object
{
	// general attributes
	t_type	type;
	t_vec	pos;
	t_vec	color;  // converted to 3d vector from rgb  ==parsing== (function is ready)
	t_vec	oc;  // vector from object to camera

	// sphere and cylinder
	double	r;  // converted to radius from diameter  ==parsing==

	// plane and cylinder
	t_vec	normal;

	// cylinder
	double	h;

}	t_object;

/* Coefficient a, b, c and discriminant delta for the quadratic equation */
typedef struct s_discrim
{
	double	a;
	double	b;
	double	c;
	double	delta;

}	t_discrim;

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
	double		rot[3][3];
	double		px;
	t_cam		cam;
	t_ambient	amb;
	t_light		light;
	t_object	*obj;  // array of objects; saved as void pointer so that it can contain different structs
	t_object	*obj_lst;  // list of objects; will be cleared up after parsing
	int			obj_id;  // current object id

}	t_info;

t_info		*get_info(void);
int			free_exit(char *s);

uint8_t		clamp(double single_channel_color);
uint32_t	vec_to_color(t_vec color);
t_vec		color_to_vec(int r, int g, int b);

void		get_rotation_matrix(t_info *info, t_vec v);
void		rotate(double rot[3][3], t_vec *v1);

void		draw(void *param);

void		move_camera(mlx_key_data_t keydata, t_info *info);
void		rotate_camera(mlx_key_data_t keydata, t_info *info);

#endif
