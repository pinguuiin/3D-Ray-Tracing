/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 23:34:48 by piyu              #+#    #+#             */
/*   Updated: 2025/09/24 00:23:45 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	color_pixel(t_info *info, int x, int y, double p)
{
	uint8_t		r;
	uint8_t		g;
	uint8_t		b;
	uint32_t	color;

	// inside the object is ignored for now
	if (p <= 0)
	{
		mlx_put_pixel(info->img, x, y, 0x000000FF);
		return ;
	}
	r = 255 - log(p) * 600;
	g = 255 - log(p) * 600;
	b = 255 - log(p) * 400;
	color = r << 24 | g << 16 | b << 8 | 255;
	mlx_put_pixel(info->img, x, y, color);
}

void	draw_pixel(t_info *info, int x, int y)
{
	t_vec		viewport;
	t_vec		ray;
	t_quad_coef	f;

	viewport = vec3(x * info->px - info->viewport_width / 2.0,
		info->viewport_height / 2.0 - y * info->px, 0);
	rotate(&viewport, info->viewport_rot);
	ray = add(info->cam.direction, viewport);
	f.a = dot(ray, ray);
	f.b = 2 * dot(ray, info->sphere.oc);
	f.c = dot(info->sphere.oc, info->sphere.oc) - info->sphere.r * info->sphere.r;
	f.delta = f.b * f.b - 4.0 * f.a * f.c;
	if (f.delta >= 0)
	{
		color_pixel(info, x, y, (- f.b - sqrt(f.delta)) / (2 * f.a)
		* norm(ray) / (norm(info->sphere.oc) - info->sphere.r));
	}
	else
		color_pixel(info, x, y, -1);
}

void	draw(void *param)
{
	t_info		*info;
	int			x;
	int			y;

	info = (t_info *)param;
	x = 0;
	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
			draw_pixel(info, x, y);
			y++;
		}
		x++;
	}
}
