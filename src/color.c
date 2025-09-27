/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 21:02:48 by piyu              #+#    #+#             */
/*   Updated: 2025/09/27 04:17:16 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

uint8_t	clamp(double single_channel_color)
{
	if (single_channel_color > 255)
		return (255);
	else if (single_channel_color < 0)
		return (0);
	else
		return ((uint8_t)single_channel_color);
}

uint32_t	vec_to_color(t_vec color)
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;

	r = clamp(round(color.x * 255));
	g = clamp(round(color.y * 255));
	b = clamp(round(color.z * 255));
	return (r << 24 | g << 16 | b << 8 | 255);
}

t_vec	color_to_vec(int r, int g, int b)
{
	t_vec	v;

	v.x = clamp(r) / 255.0;
	v.y = clamp(g) / 255.0;
	v.z = clamp(b) / 255.0;
	return (v);
}
