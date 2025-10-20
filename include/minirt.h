/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 20:48:54 by piyu              #+#    #+#             */
/*   Updated: 2025/10/22 02:56:49 by piyu             ###   ########.fr       */
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
#  define KD 0.5
# endif
# ifndef SHININESS
#  define SHININESS 30
# endif

# include "../src/libft/libft.h"
# include "../src/MLX42/include/MLX42/MLX42.h"
# include "parser.h"
# include "get_next_line_minirt.h"
# include "vector.h"
# include "hit.h"
# include <stdlib.h>
# include <math.h>
# include <stdbool.h>

/* Type of object */
typedef enum e_type
{
	SPHERE,
	PLANE,
	CYLINDER

}	t_type;

/* Key light */
typedef struct s_light
{
	t_vec	pos;
	t_vec	color;

}	t_light;

/* Camera specs */
typedef struct s_cam
{
	double	fov; // converted to rad from degree  ==parsing== (multiply the provided angle by π / 180)
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
	double	h;  // Half height of the cylinder

}	t_object;

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
	t_vec		amb;
	t_cam		cam;
	t_light		light;
	t_object	*obj;  // array of objects; saved as void pointer so that it can contain different structs
	t_object	*obj_lst;  // list of objects; will be cleared up after parsing
	int			num;  // number of objects
	bool		is_inside;

}	t_info;

t_info		*get_info(void);
int			free_exit(char *s);

uint8_t		clamp(double single_channel_color);
uint32_t	vec_to_color(t_vec color);
t_vec		color_to_vec(int r, int g, int b);

double		ray_hit_sphere(t_info *info, t_vec ray, t_object *sphere, t_vec pos);
double		ray_hit_plane(t_vec ray, t_object *plane, t_vec pos);
double		ray_hit_cylinder(t_info *info, t_vec ray, t_object *cy, t_vec pos);
void		renderer(void *param);

t_vec		reflection(t_info *info, t_object *obj, t_vec ray, t_hit *hit);

// void		rotate_x(t_vec *vec, double theta);
void		rotate_y(t_vec *vec, double theta);
// void		rotate_z(t_vec *vec, double theta);
void		get_rotation_matrix(t_info *info, t_vec v);
void		rotate(double rot[3][3], t_vec *v1);

void		move_camera(mlx_key_data_t keydata, t_info *info);
void		rotate_camera(mlx_key_data_t keydata, t_info *info);

#endif
