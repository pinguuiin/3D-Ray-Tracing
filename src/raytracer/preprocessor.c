/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocessor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 04:45:19 by piyu              #+#    #+#             */
/*   Updated: 2025/11/16 22:57:32 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	update_oc_and_plane_normal(t_info *info)
{
	int i;

	i = 0;
	while (i < info->n_obj)
	{
		info->obj[i].oc = subtract(info->cam.pos, info->obj[i].pos);
		if (info->obj[i].type == PLANE &&
			dot(info->obj[i].normal, info->obj[i].oc) < 0)
			info->obj[i].normal = scale(info->obj[i].normal, -1);
		i++;
	}
}

void	get_viewport_data(t_info *info)
{
	info->focal_length = 1.0;
	info->viewport_width = tan(info->cam.fov / 2.0) * 2 * info->focal_length;
	info->viewport_height = info->viewport_width * info->height / info->width;
	info->px = info->viewport_width / info->width;
}

void	preprocessor(t_info *info)
{
	info->width = WIDTH;
	info->height = HEIGHT;
	update_oc_and_plane_normal(info);
	get_rotation_matrix(info, info->cam.direction);
	get_viewport_data(info);
}
