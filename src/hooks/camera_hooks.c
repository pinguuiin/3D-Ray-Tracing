/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:24:03 by piyu              #+#    #+#             */
/*   Updated: 2025/11/04 04:30:35 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

inline void	move_camera(mlx_key_data_t keydata, t_info *info)
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
	update_oc_and_plane_normal(info);
}

inline void	rotate_camera(mlx_key_data_t keydata, t_info *info)
{
	// incorrect increment direction
	if (keydata.key == MLX_KEY_UP)
		info->cam.direction.y += 0.1;
	else if (keydata.key == MLX_KEY_DOWN)
		info->cam.direction.y -= 0.1;
	else if (keydata.key == MLX_KEY_RIGHT)
		rotate_y(&info->cam.direction, M_PI / 6.0);  // info->cam.direction.x += 0.1;
	else if (keydata.key == MLX_KEY_LEFT)
		rotate_y(&info->cam.direction, -M_PI / 6.0);  // info->cam.direction.x -= 0.1;
	info->cam.direction = normalize(info->cam.direction);
	get_rotation_matrix(info, info->cam.direction);
}
