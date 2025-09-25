/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 20:48:17 by piyu              #+#    #+#             */
/*   Updated: 2025/09/25 15:27:34 by piyu             ###   ########.fr       */
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
	t_info		info;
	t_sphere	sphere;

	info.cam.fov = 120.0 / 180.0 * M_PI;
	info.cam.pos.x = 0.0;
	info.cam.pos.y = 0.0;
	info.cam.pos.z = 0.0;
	info.cam.direction.x = 2.5;
	info.cam.direction.y = 0.0;
	info.cam.direction.z = 4.0;
	info.cam.direction = normalize(info.cam.direction);
	get_viewport_rotation(&info, vec3(0.0, 0.0, 1.0), info.cam.direction);
	sphere.type = SPHERE;
	sphere.r = 4.0;
	sphere.pos.x = 0.0;
	sphere.pos.y = 0.0;
	sphere.pos.z = 10.0;
	sphere.oc = subtract(info.cam.pos, sphere.pos);
	info.object = &sphere;
	info.focal_length = 1.0;
	info.viewport_width = tan(info.cam.fov / 2.0) * 2 * info.focal_length;
	info.viewport_height = info.viewport_width * (double)HEIGHT / (double)WIDTH;
	info.px = info.viewport_width / (double)WIDTH;
	initialize_mlx(&info);
	mlx_key_hook(info.mlx, &escape_handler, info.mlx);
	mlx_loop_hook(info.mlx, draw, &info);
	mlx_loop(info.mlx);
	free_exit(&info, NULL);
	return (0);
}
