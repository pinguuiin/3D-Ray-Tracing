/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 22:36:08 by piyu              #+#    #+#             */
/*   Updated: 2025/12/10 00:22:23 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

#ifndef BONUS
#else
inline void	rotate_object(mlx_key_data_t keydata, t_info *info)
{
	if (keydata.key == MLX_KEY_1)
		info->auto_rotate = true;
	else
		info->auto_rotate = false;
}
#endif
