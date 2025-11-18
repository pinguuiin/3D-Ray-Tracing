/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 20:48:54 by piyu              #+#    #+#             */
/*   Updated: 2025/11/18 23:16:12 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# define WIDTH 1500
# define HEIGHT 1000
# define EPSILON 1e-8
# define KS 0.5
# define KD 0.5
# define SHININESS 30

# include "../src/libft/libft.h"
# include "../src/MLX42/include/MLX42/MLX42.h"
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
	t_color	color;

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
	t_color	color;  // converted to 3d vector from rgb  ==parsing== (function is ready)
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
	double		viewport_w;
	double		viewport_h;
	double		rot[3][3];
	double		px;
	t_color		amb;
	t_cam		cam;
	t_light		*light;	// array of lights
	t_object	*obj;  	// array of objects
	int			n_light;
	int			n_obj;
	bool		is_inside;

}	t_info;

t_info		*get_info(void);
int			free_exit(char *s);
void		resize(int32_t width, int32_t height, void *param);

uint8_t		clamp(double single_channel_color);
uint32_t	vec_to_color(t_vec color);
// t_vec		color_to_vec(int r, int g, int b);

double		ray_hit_sphere(t_info *info, t_vec ray, t_object *sphere, t_vec oc);
double		ray_hit_plane(t_vec ray, t_object *plane, t_vec oc);
double		ray_hit_cylinder(t_info *info, t_vec ray, t_object *cy, t_vec oc);
void		renderer(void *param);

t_vec		reflection(t_info *info, t_object *obj, t_vec ray, t_hit *hit);

// void		rotate_x(t_vec *vec, double theta);
void		rotate_y(t_vec *vec, double theta);
// void		rotate_z(t_vec *vec, double theta);
void		get_rotation_matrix(t_info *info, t_vec v);
void		rotate(double rot[3][3], t_vec *v1);

void		key_handler(mlx_key_data_t keydata, void *param);

void		update_oc_and_plane_normal(t_info *info);
void		get_viewport_data(t_info *info);
void		preprocessor(t_info *info);

#endif
