/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 22:42:18 by piyu              #+#    #+#             */
/*   Updated: 2025/12/31 05:34:14 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	resize(int32_t width, int32_t height, void *param)
{
	t_info	*info;

	info = (t_info *)param;
	if (mlx_resize_image(info->img, width, height) == false)
		exit(free_exit("Image resizing failed", MLX_FAILURE));
	get_viewport_data(info);
}

#ifndef BONUS
#else

void	adjust_ray_depth(mlx_key_data_t keydata, void *param)
{
	t_info	*info;

	if (keydata.action != MLX_PRESS)
		return ;
	info = (t_info *)param;
	if (keydata.key == MLX_KEY_B && info->ray_depth < 5)
		info->ray_depth++;
	else if (keydata.key == MLX_KEY_N && info->ray_depth > 1)
		info->ray_depth--;
}

inline void	adjust_ambient_brightness(mlx_key_data_t keydata, void *param)
{
	t_info	*info;

	if (keydata.action != MLX_PRESS)
		return ;
	info = (t_info *)param;
	if (keydata.key == MLX_KEY_L && info->amb.r < 0.83 && info->amb.g < 0.83
		&& info->amb.b < 0.83)
		info->amb = scale(info->amb, 1.20);
	else if (keydata.key == MLX_KEY_K && norm(info->amb) > 0.1)
		info->amb = scale(info->amb, 0.83);
}
#endif
