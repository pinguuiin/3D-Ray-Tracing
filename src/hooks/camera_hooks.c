/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:24:03 by piyu              #+#    #+#             */
/*   Updated: 2025/11/17 00:57:39 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// TODO: attempt at making the camera move according to the direction it is facing.
// FIXME: only up and down (along the 'y' axis) needs work. the others are working really well!
static inline void	move_camera(mlx_key_data_t keydata, t_info *info)
{
	t_vec	right_vector;

	if (keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_A)
	{
		right_vector = normalize(cross(info->cam.direction, vec3(0.0, 1.0, 0.0)));
		if (keydata.key == MLX_KEY_D)
			info->cam.pos = add(info->cam.pos, dot_elem(right_vector, vec3(0.8, 0.8, 0.8)));
		else
			info->cam.pos = subtract(info->cam.pos, dot_elem(right_vector, vec3(0.8, 0.8, 0.8)));
	}
	else if (keydata.key == MLX_KEY_Q)
		info->cam.pos.y += 0.4;
	else if (keydata.key == MLX_KEY_Z)
		info->cam.pos.y -= 0.4;
	else if (keydata.key == MLX_KEY_W)
		info->cam.pos = add(info->cam.pos, dot_elem(info->cam.direction, vec3(0.8, 0.8, 0.8)));
	else if (keydata.key == MLX_KEY_S)
		info->cam.pos = subtract(info->cam.pos, dot_elem(info->cam.direction, vec3(0.8, 0.8, 0.8)));
	info->has_moved = 1;
	// update_oc_and_plane_normal(info); // NOTE: this is finally done from the renderer!
}

/*
static inline void	move_camera(mlx_key_data_t keydata, t_info *info)
{
	if (keydata.key == MLX_KEY_D)
		info->cam.pos.x += 0.2;
	else if (keydata.key == MLX_KEY_A)
		info->cam.pos.x -= 0.2;
	else if (keydata.key == MLX_KEY_Q)
		info->cam.pos.y += 0.2;
	else if (keydata.key == MLX_KEY_Z)
		info->cam.pos.y -= 0.2;
	else if (keydata.key == MLX_KEY_W)
		info->cam.pos.z += 0.2;
	else if (keydata.key == MLX_KEY_S)
		info->cam.pos.z -= 0.2;
	info->has_moved = 1;
	// update_oc_and_plane_normal(info); // NOTE: this is finally done from the renderer!
}
*/

static inline void	rotate_camera(mlx_key_data_t keydata, t_info *info)
{
	// incorrect increment direction
	if (keydata.key == MLX_KEY_UP && info->cam.direction.y < 0.98)
		info->cam.direction.y += 0.05;
	else if (keydata.key == MLX_KEY_DOWN && info->cam.direction.y > -0.98)
		info->cam.direction.y -= 0.05;
	else if (keydata.key == MLX_KEY_RIGHT)
		rotate_y(&info->cam.direction, M_PI / 60.0);  // info->cam.direction.x += 0.1;
	else if (keydata.key == MLX_KEY_LEFT)
		rotate_y(&info->cam.direction, -M_PI / 60.0);  // info->cam.direction.x -= 0.1;
	// info->cam.direction = normalize(info->cam.direction);
	// get_rotation_matrix(info, info->cam.direction);
	info->has_moved = 1;
}

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
