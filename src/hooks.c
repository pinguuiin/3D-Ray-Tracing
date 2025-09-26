/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:24:03 by piyu              #+#    #+#             */
/*   Updated: 2025/09/26 16:43:01 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	move_camera(mlx_key_data_t keydata, t_info *info)
{
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
	((t_sphere *)info->obj)->oc = subtract(info->cam.pos, ((t_sphere *)info->obj)->pos);
	draw(info);
}
