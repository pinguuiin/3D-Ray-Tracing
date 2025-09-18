/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 20:48:17 by piyu              #+#    #+#             */
/*   Updated: 2025/09/18 20:22:42 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	get_viewport_coordinates(t_info *info)
{
	int		x;
	int		y;
	double	px;

	px = tan(info->cam.fov / 2.0) * 2 / (double)(WIDTH - 1);
	x = 0;
	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
			info->cam.viewport[x][y] = add(info->cam.normal, vec3(x *
				px - WIDTH / 2.0, HEIGHT / 2.0 - y * px, 0));
			y++;
		}
		x++;
	}
}

int	draw(t_info *info)
{
	int		x;
	int		y;
	double	delta;
	t_vec	d;

	get_viewport_coordinates(info);
	d = subtract(info->cam.pos, info->sphere.pos);
	x = 0;
	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
			delta = 4.0 * dot(info->cam.viewport[x][y], d) *
				dot(info->cam.viewport[x][y], d) - 4.0 *
				dot(info->cam.viewport[x][y], info->cam.viewport[x][y]) *
				(dot(d, d) - pow(info->sphere.r, 2));
			y++;
		}
		x++;
	}
}

int	main(void)
{
}
