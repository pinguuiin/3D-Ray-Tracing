/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 22:42:18 by piyu              #+#    #+#             */
/*   Updated: 2025/12/03 07:38:04 by piyu             ###   ########.fr       */
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

static inline void	update_camera_for_new_frame(t_info *info)
{
	if (info->has_rotated)
	{
		info->has_rotated = 0;
		info->cam_curr_frame.direction = info->cam.direction;
		get_rotation_matrix(info->rot, info->cam_curr_frame.direction, vec3(0, 1, 0));
	}
	if (info->has_moved)
	{
		info->has_moved = 0;
		info->cam_curr_frame.pos = info->cam.pos;
		update_oc_and_plane_normal(info);
	}
}

#ifndef BONUS
void	update_data_for_new_frame(t_info *info)
{
	update_camera_for_new_frame(info);
	if ((uint32_t) info->mlx->height != info->img->height
		|| (uint32_t)info->mlx->width != info->img->width)
		resize(info->mlx->width, info->mlx->height, info);
	info->is_inside = false;
}
#else
void	update_data_for_new_frame(t_info *info)
{
	int	i;

	update_camera_for_new_frame(info);
	if ((uint32_t) info->mlx->height != info->img->height
		|| (uint32_t)info->mlx->width != info->img->width)
	{
		resize(info->mlx->width, info->mlx->height, info);
		if (atomic_load(&info->thread_system.is_multithreaded))
		{
			i = 0;
			while (i < N_THREADS)
			{
				init_chunk_borders(info->img->width,
					&info->thread_system.threads[i], i);
				i++;
			}
		}
	}
	info->is_inside = false;
}
#endif
