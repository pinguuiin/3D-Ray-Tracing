/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 05:33:21 by piyu              #+#    #+#             */
/*   Updated: 2025/12/15 05:34:26 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline void	update_camera_for_new_frame(t_info *info)
{
	if (info->has_rotated)
	{
		info->has_rotated = 0;
		info->cam_curr_frame.direction = info->cam.direction;
		get_rotation_matrix(info->rot, info->cam_curr_frame.direction,
			vec3(0, 1, 0));
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
}
#else

static inline void	update_sphere_phase_for_new_frame(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->n_obj)
	{
		if (info->obj[i].type == SPHERE)
			info->obj[i].phase -= 0.5 * info->mlx->delta_time;
		i++;
	}
}

void	update_data_for_new_frame(t_info *info)
{
	int	i;

	update_camera_for_new_frame(info);
	if (info->auto_rotate == true)
		update_sphere_phase_for_new_frame(info);
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
}
#endif