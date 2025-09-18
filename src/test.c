/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 20:48:17 by piyu              #+#    #+#             */
/*   Updated: 2025/09/18 20:29:45 by piyu             ###   ########.fr       */
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
			if (delta >= 0)
				color_object();
			else
				color_background();
			y++;
		}
		x++;
	}
}

void	initialize_mlx(t_info *info)
{
	mlx_set_setting(MLX_STRETCH_IMAGE, 1);
	fdf->mlx = mlx_init(WIN_W, WIN_H, "Fdf", true);
	if (!fdf->mlx)
		free_exit(fdf, "Instance initializing failed", 0);
	fdf->img = mlx_new_image(fdf->mlx, WIN_W, WIN_H);
	if (!fdf->img)
		free_exit(fdf, "Creating new image buffer failed", 0);
	if (mlx_image_to_window(fdf->mlx, fdf->img, 0, 0) == -1)
		free_exit(fdf, "Drawing image on the window failed", 0);
}

int	main(void)
{
}
