/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:24:03 by piyu              #+#    #+#             */
/*   Updated: 2025/10/04 01:39:59 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	move_camera(mlx_key_data_t keydata, t_info *info)
{
	int	i;

	i = 0;
	if (keydata.key == MLX_KEY_RIGHT)
		info->cam.pos.x += 1;
	else if (keydata.key == MLX_KEY_LEFT)
		info->cam.pos.x -= 1;
	else if (keydata.key == MLX_KEY_UP)
		info->cam.pos.y += 1;
	else if (keydata.key == MLX_KEY_DOWN)
		info->cam.pos.y -= 1;
	else if (keydata.key == MLX_KEY_Q)
		info->cam.pos.z += 1;
	else if (keydata.key == MLX_KEY_Z)
		info->cam.pos.z -= 1;
	while (i < info->num)
	{
		info->obj[i].oc = subtract(info->cam.pos, info->obj[i].pos);
		if (info->obj[i].type == PLANE &&
			dot(info->obj[i].normal, info->obj[i].oc) < 0)
			info->obj[i].normal = scale(info->obj[i].normal, -1);
		i++;
	}
}

void	rotate_camera(mlx_key_data_t keydata, t_info *info)
{
	// incorrect increment direction
	if (keydata.key == MLX_KEY_W)
		info->cam.direction.y += 0.1;
	else if (keydata.key == MLX_KEY_S)
		info->cam.direction.y -= 0.1;
	else if (keydata.key == MLX_KEY_D)
		rotate_y(&info->cam.direction, M_PI / 6.0);  // info->cam.direction.x += 0.1;
	else if (keydata.key == MLX_KEY_A)
		rotate_y(&info->cam.direction, -M_PI / 6.0);  // info->cam.direction.x -= 0.1;
	info->cam.direction = normalize(info->cam.direction);
	get_rotation_matrix(info, info->cam.direction);
}
