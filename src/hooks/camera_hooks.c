/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:24:03 by piyu              #+#    #+#             */
/*   Updated: 2025/12/31 02:41:34 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
* This key hook allows for movement of the camera relevant to the direction it
* is facing. Because of that, it first has to check whether a rotation of the
* camera has taken place - so that it can use updated values for its up, right
* and forwards vectors, thanks to the call to get_rotation_matrix().
*
* 'f', 'r' and 'u' vectors: please refer to get_rotation_matrix() in rotation.c
* for a clearer understanding of those vectors, all stored within 'info->rot'.
* Movement along the camera's relative Z axis needs to use the 'forward' vector,
* which is already stored in the camera's direction.
* Movement along the camera's relative X axis needs to use the 'right' vector,
* stored within the rotation matrix 'rot' (x, y, z coordinates: [0][0], [1][0]
* and [2][0]).
* And movement along the camera's relative Y axis uses the 'up' vector, stored
* as well within the rotation matric - [0][1], [1][1] and [2][1].
*
* usage:
* - 'W' and 'S': forward and backward movement
* - 'D' and 'A': right and left movement
* - 'Q' and 'Z':  up and down movement
*/
static inline void	move_camera(mlx_key_data_t keydata, t_info *info)
{
	double	(*rot)[3];
	t_cam	*cam;

	cam = &info->cam;
	info->has_moved = 1;
	if (info->has_rotated)
		get_rotation_matrix(info->rot, cam->direction, vec3(0, 1, 0));
	rot = info->rot;
	if (keydata.key == MLX_KEY_W)
		cam->pos = add(cam->pos, scale(cam->direction, 0.5));
	else if (keydata.key == MLX_KEY_S)
		cam->pos = subtract(cam->pos, scale(cam->direction, 0.5));
	else if (keydata.key == MLX_KEY_D)
		cam->pos = add(cam->pos,
				vec3(rot[0][0] * 0.5, rot[1][0] * 0.5, rot[2][0] * 0.5));
	else if (keydata.key == MLX_KEY_A)
		cam->pos = subtract(cam->pos,
				vec3(rot[0][0] * 0.5, rot[1][0] * 0.5, rot[2][0] * 0.5));
	else if (keydata.key == MLX_KEY_Q)
		cam->pos = add(cam->pos,
				vec3(rot[0][1] * 0.5, rot[1][1] * 0.5, rot[2][1] * 0.5));
	else if (keydata.key == MLX_KEY_Z)
		cam->pos = subtract(cam->pos,
				vec3(rot[0][1] * 0.5, rot[1][1] * 0.5, rot[2][1] * 0.5));
}

static inline void	rotate_camera(mlx_key_data_t keydata, t_info *info)
{
	t_cam	*cam;

	cam = &info->cam;
	info->has_rotated = 1;
	if (keydata.key == MLX_KEY_UP && cam->direction.y < 0.98)
		cam->direction.y += 0.05;
	else if (keydata.key == MLX_KEY_DOWN && cam->direction.y > -0.98)
		cam->direction.y -= 0.05;
	else if (keydata.key == MLX_KEY_RIGHT)
		rotate_y(&cam->direction, M_PI / 60.0);
	else if (keydata.key == MLX_KEY_LEFT)
		rotate_y(&cam->direction, -M_PI / 60.0);
	cam->direction = normalize(cam->direction);
}

#ifndef BONUS

void	key_handler(mlx_key_data_t keydata, void *param)
{
	t_info	*info;

	info = (t_info *)param;
	if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(info->mlx);
	else if (keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_A
		|| keydata.key == MLX_KEY_Q || keydata.key == MLX_KEY_Z
		|| keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_S)
		move_camera(keydata, info);
	else if (keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_DOWN
		|| keydata.key == MLX_KEY_RIGHT || keydata.key == MLX_KEY_LEFT)
		rotate_camera(keydata, info);
}
#else

void	key_handler(mlx_key_data_t keydata, void *param)
{
	t_info	*info;

	info = (t_info *)param;
	if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(info->mlx);
	else if (keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_A
		|| keydata.key == MLX_KEY_Q || keydata.key == MLX_KEY_Z
		|| keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_S)
		move_camera(keydata, info);
	else if (keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_DOWN
		|| keydata.key == MLX_KEY_RIGHT || keydata.key == MLX_KEY_LEFT)
		rotate_camera(keydata, info);
	else if (keydata.key == MLX_KEY_1 || keydata.key == MLX_KEY_2)
		rotate_object(keydata, info);
	else if (keydata.key == MLX_KEY_B || keydata.key == MLX_KEY_N)
		adjust_ray_depth(keydata, info);
	else if (keydata.key == MLX_KEY_L || keydata.key == MLX_KEY_K)
		adjust_ambient_brightness(keydata, info);
}
#endif
