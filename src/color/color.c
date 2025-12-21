/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 21:02:48 by piyu              #+#    #+#             */
/*   Updated: 2025/12/21 03:43:20 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

uint8_t	clamp(double single_channel_color)
{
	if (single_channel_color > 255.0)
		return (255);
	else if (single_channel_color < 0.0)
		return (0);
	else
		return ((uint8_t)single_channel_color);
}

inline uint32_t	vec_to_color(t_vec color)
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;

	r = clamp(color.x * 255);
	g = clamp(color.y * 255);
	b = clamp(color.z * 255);
	return (r << 24 | g << 16 | b << 8 | 255);
}
