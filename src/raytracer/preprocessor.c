/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocessor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 04:45:19 by piyu              #+#    #+#             */
/*   Updated: 2025/11/26 17:18:28 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

inline void	update_oc_and_plane_normal(t_info *info)
{
	int i;

	i = 0;
	while (i < info->n_obj)
	{
		info->obj[i].oc = subtract(info->cam_curr_frame.pos, info->obj[i].pos);
		if (info->obj[i].type == PLANE &&
			dot(info->obj[i].axis, info->obj[i].oc) < 0)
			info->obj[i].axis = scale(info->obj[i].axis, -1);
		i++;
	}
}

inline void	get_viewport_data(t_info *info)
{
	info->focal_length = 1.0;
	info->viewport_w = tan(info->cam_curr_frame.fov / 2.0) * 2 * info->focal_length;
	info->viewport_h = info->viewport_w * info->img->height / info->img->width;
	info->px = info->viewport_w / info->img->width;
}

#ifndef BONUS
void	preprocessor(t_info *info)
{
	info->cam_curr_frame = info->cam;
	update_oc_and_plane_normal(info);
	get_rotation_matrix(info, info->cam_curr_frame.direction);
	get_viewport_data(info);
}
#else
void	preprocessor(t_info *info)
{
	int	i;

	i = 0;
	info->cam_curr_frame = info->cam;
	update_oc_and_plane_normal(info);
	get_rotation_matrix(info, info->cam_curr_frame.direction);
	get_viewport_data(info);
	while (i < info->n_obj)
	{
		parse_texture(&info->obj[i], "limestone3");   // name will later be passed from input ==========================c:=====c:=====c:=====c:=================
		i++;
	}
}
#endif
