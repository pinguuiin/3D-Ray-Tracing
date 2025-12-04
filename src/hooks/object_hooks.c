/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 22:36:08 by piyu              #+#    #+#             */
/*   Updated: 2025/11/30 01:43:47 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

#ifndef BONUS
#else
inline void	rotate_object(mlx_key_data_t keydata, t_info *info)
{
	int	i;

	i = 0;
	while (i <= info->n_obj && i <= 5)
	{
		if (keydata.key == (keys_t)(MLX_KEY_1 + i) && info->obj[i].type == SPHERE)
		{
			info->obj[i].phase -= 0.02;
			return ;
		}
		i++;
	}
}
#endif
