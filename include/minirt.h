/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 20:48:54 by piyu              #+#    #+#             */
/*   Updated: 2025/12/03 08:09:06 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# define WIDTH 1500
# define HEIGHT 1000
# define EPSILON 1e-8
# define KS 0.05
# define KD 2
# define SHININESS 30

# include "../libft/libft.h"
# include "../MLX42/include/MLX42/MLX42.h"
# include "vector.h"
# include "hit.h"

# include <stdlib.h>
# include <math.h>
# include <stdbool.h>

# ifndef BONUS
# else
#  include "multithreading.h"
# endif

/*
* Exit codes:
*	1: MLX function failures
*	2: invalid input - unexpected argument or misconfigured .rt file
*	3: fatal system error during parsing, such as failures of open(), malloc(),
* 	   read(), close() and pthread_create() functions, or buffer for
* 	   get_next_line_revised() is predefined as empty.
*	0: If program runs smoothly
*/
enum e_exit_code
{
	SUCCESS			=	0,
	MLX_FAILURE		=	1,
	INPUT_ERROR		=	2,
	SYSTEM_FAILURE	=	3

};

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

# ifndef BONUS
	/* Object information */
	typedef struct s_object
	{
		// general attributes
		t_type	type;
		t_vec	pos;
		t_color	color;
		t_vec	oc;  // vector from object to camera

		// sphere and cylinder
		double	r;

		// plane and cylinder
		t_vec	axis;

		// cylinder
		double	h;  // Half height of the cylinder

	}	t_object;
# else
	/* Object information */
	typedef struct s_object
	{
		// general attributes
		t_type			type;
		t_vec			pos;
		t_color			color;
		t_vec			oc;  // vector from object to camera

		// sphere and cylinder
		double			r;

		// plane and cylinder
		t_vec			axis;

		// cylinder
		double			h;  // Half height of the cylinder

		// texture
		bool			has_tex;
		char			*tex_name;
		char			*tex_file;
		char			*normal_file;
		mlx_texture_t	*texture;
		mlx_texture_t	*normal;
		double			rot[3][3];
		double			phase;

	}	t_object;
# endif

# ifndef BONUS
	// Struct that includes everything
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
		t_cam		cam_curr_frame;
		t_object	*obj;  	// array of objects
		int			n_obj;
		t_light		light;	// one single light
		bool		is_inside;
		bool		has_moved;
		bool		has_rotated;


	}	t_info;
# else
	// Struct that includes everything, as well as data for multithreading
	typedef struct s_info
	{
		mlx_t			*mlx;
		mlx_image_t		*img;
		double			focal_length;
		double			viewport_w;
		double			viewport_h;
		double			rot[3][3];
		double			px;
		t_color			amb;
		t_cam			cam;
		t_cam			cam_curr_frame;
		t_object		*obj;  	// array of objects
		int				n_obj;
		t_light			*light;	// array of lights
		int				n_light;
		bool			is_inside;
		bool			has_moved;
		bool			has_rotated;
		t_thread_system	thread_system;

	}	t_info;
# endif

t_info		*get_info(void);
void		resize(int32_t width, int32_t height, void *param);
int			free_exit(char *s, int exit_code);

uint8_t		clamp(double single_channel_color);
uint32_t	vec_to_color(t_vec color);
// t_vec		color_to_vec(int r, int g, int b);
#ifndef BONUS
#else
void		sphere_xyz_to_px_loc(t_vec p, t_object *sphere, int *i, int *j);
t_color		px_loc_to_color(mlx_texture_t *map, int i, int j);
t_vec		px_loc_to_normal(mlx_texture_t *map, int i, int j);
void		parse_texture(t_object *obj, char *name);
void		get_object_rot_matrix(double (*rot)[3], t_vec u);
void		rotate_object(mlx_key_data_t keydata, t_info *info);
void		sphere_tbn_to_xyz(t_object *obj, t_hit *hit);
#endif

double		ray_hit_sphere(t_info *info, t_vec ray, t_object *sphere, t_vec oc);
double		ray_hit_plane(t_vec ray, t_object *plane, t_vec oc);
double		ray_hit_cylinder(t_info *info, t_vec ray, t_object *cy, t_vec oc);
void		renderer(void *param);
void		render_column(int x, t_info *info);
void		update_data_for_new_frame(t_info *info);

t_vec		reflection(t_info *info, t_object *obj, t_vec ray, t_hit *hit);

// void		rotate_x(t_vec *vec, double theta);
void		rotate_y(t_vec *vec, double theta);
// void		rotate_z(t_vec *vec, double theta);
void		get_rotation_matrix(double (*rot)[3], t_vec f, t_vec up);
void		rotate(double rot[3][3], t_vec *v1);

void		key_handler(mlx_key_data_t keydata, void *param);

void		update_oc_and_plane_normal(t_info *info);
void		get_viewport_data(t_info *info);
void		preprocessor(t_info *info);


#endif
