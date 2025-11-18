/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 22:42:18 by piyu              #+#    #+#             */
/*   Updated: 2025/11/18 23:07:19 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	resize(int32_t width, int32_t height, void *param)
{
	t_info	*info;

	info = (t_info *)param;
	if (mlx_resize_image(info->img, width, height) == false)
		exit(free_exit("Image resizing failed"));
	get_viewport_data(info);
}
