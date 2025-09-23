/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 20:48:17 by piyu              #+#    #+#             */
/*   Updated: 2025/09/20 01:42:40 by piyu             ###   ########.fr       */
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
	{
		ft_putendl_fd("Error", 2);
		ft_putendl_fd(s, 2);
	}
	return (1);
}

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

void	draw(t_info *info)
{
	int			x;
	int			y;
	t_quad_coef	f;
	t_vec		viewport;
	t_vec		ray;

	x = 0;
	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
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
			y++;
		}
		x++;
	}
}

static void	escape_handler(mlx_key_data_t keydata, void *param)
{
	mlx_t	*mlx;

	mlx = NULL;
	mlx = (mlx_t *)param;
	if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(mlx);
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

	info.cam.fov = 120.0 / 180.0 * M_PI;
	info.cam.pos.x = 0.0;
	info.cam.pos.y = 0.0;
	info.cam.pos.z = 0.0;
	info.cam.direction.x = 2.5;
	info.cam.direction.y = 0.0;
	info.cam.direction.z = 4.0;
	info.cam.direction = normalize(info.cam.direction);
	get_viewport_rotation(&info, vec3(0.0, 0.0, 1.0), info.cam.direction);
	info.sphere.r = 4.0;
	info.sphere.pos.x = 0.0;
	info.sphere.pos.y = 0.0;
	info.sphere.pos.z = 10.0;
	info.sphere.oc = subtract(info.cam.pos, info.sphere.pos);
	info.focal_length = 1.0;
	info.viewport_width = tan(info.cam.fov / 2.0) * 2 * info.focal_length;
	info.viewport_height = info.viewport_width * (double)HEIGHT / (double)WIDTH;
	info.px = info.viewport_width / (double)WIDTH;
	initialize_mlx(&info);
	mlx_key_hook(info->mlx, &escape_handler, info->mlx);
	draw(&info);
	mlx_loop(info.mlx);
	free_exit(&info, NULL);
	return (0);
}
