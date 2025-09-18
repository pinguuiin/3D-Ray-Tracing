/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 20:48:17 by piyu              #+#    #+#             */
/*   Updated: 2025/09/18 23:45:41 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	free_exit(t_info *info, char *s)
{
	if (info->img)
		mlx_delete_image(info->mlx, info->img);
	if (info->mlx)
		mlx_terminate(info->mlx);
	if (s)
		ft_putendl_fd(s, 2);
	return (1);
}

void	get_viewport_coordinates(t_info *info)
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

void	color_pixel(t_info *info, int x, int y, double d)
{
	// inside the object is ignored for now
	if (d <= 0)
		return ;
}

void	draw(t_info *info)
{
	int			x;
	int			y;
	t_vec		oc;
	double		delta;
	t_quad_coef	f;

	get_viewport_coordinates(info);
	oc = subtract(info->cam.pos, info->sphere.pos);
	x = 0;
	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
			f.a = dot(info->cam.viewport[x][y], info->cam.viewport[x][y]);
			f.b = 2 * dot(info->cam.viewport[x][y], oc);
			f.c = dot(oc, oc) - pow(info->sphere.r, 2);
			delta = f.b * f.b - 4.0 * f.a * f.c;
			if (delta >= 0)
				color_pixel(info, x, y, (- b - sqrt(delta)) / (2 * a));
			else
				color_pixel(info, x, y, -1);
			y++;
		}
		x++;
	}
}

void	initialize_mlx(t_info *info)
{
	info->mlx = NULL;
	info->img = NULL;
	mlx_set_setting(MLX_STRETCH_IMAGE, 1);
	info->mlx = mlx_init(WIDTH, HEIGHT, "miniRT", true);
	if (!info->mlx)
		exit(free_exit(info, "Instance initialization failed"));
	info->img = mlx_new_image(info->mlx, WIDTH, HEIGHT);
	if (!info->img)
		exit(free_exit(info, "Image buffer creation failed"));
	if (mlx_image_to_window(info->mlx, info->img, 0, 0) == -1)
		exit(free_exit(info, "Pushing image to window failed"));
}

int	main(void)
{
	t_info	info;

	initialize_mlx(&info);
	draw(&info);
	free_exit(&info, NULL);
	return (0);
}
