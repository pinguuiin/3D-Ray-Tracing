/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 20:48:17 by piyu              #+#    #+#             */
/*   Updated: 2025/09/28 00:25:07 by piyu             ###   ########.fr       */
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

static void	key_handler(mlx_key_data_t keydata, void *param)
{
	t_info	*info;

	info = (t_info *)param;
	if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(info->mlx);
	else if (keydata.key == MLX_KEY_LEFT || keydata.key == MLX_KEY_RIGHT
		|| keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_DOWN
		|| keydata.key == MLX_KEY_Q || keydata.key == MLX_KEY_Z)
		move_camera(keydata, info);
	else if (keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_S
		|| keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_D)
		rotate_camera(keydata, info);
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

	info.amb.ratio = 0.2;
	info.amb.color = vec3(1.0, 1.0, 1.0);

	info.light.pos = vec3(0.0, 2.0, 20.0);
	info.light.ratio = 0.8;
	info.light.color = vec3(0.5, 0.5, 1.0);

	info.cam.fov = 120.0 / 180.0 * M_PI;
	info.cam.pos = vec3(0.0, 0.0, 0.0);
	info.cam.direction = vec3(0.0, 0.0, 4.0);
	info.cam.direction = normalize(info.cam.direction);
	get_viewport_rotation(&info, vec3(0.0, 0.0, 1.0), info.cam.direction);

	info.obj = malloc(2 * sizeof(t_object));
	info.obj[0].type = SPHERE;
	info.obj[0].pos = vec3(0.0, 0.0, 10.0);
	info.obj[0].oc = subtract(info.cam.pos, info.obj[0].pos);
	info.obj[0].r = 5.0;
	info.obj[0].color = vec3(0.0, 1.0, 0.0);

	info.obj[1].type = PLANE;
	info.obj[1].pos = vec3(0.0, -3.0, 30.0);
	info.obj[1].oc = subtract(info.cam.pos, info.obj[1].pos);
	info.obj[1].normal = vec3(0.0, 10.0, -1.0);
	info.obj[1].color = vec3(1.0, 1.0, 1.0);
	info.obj_id = 1;

	info.focal_length = 1.0;
	info.viewport_width = tan(info.cam.fov / 2.0) * 2 * info.focal_length;
	info.viewport_height = info.viewport_width * (double)HEIGHT / (double)WIDTH;
	info.px = info.viewport_width / (double)WIDTH;

	initialize_mlx(&info);
	mlx_key_hook(info.mlx, &key_handler, &info);
	mlx_loop_hook(info.mlx, draw, &info);
	mlx_loop(info.mlx);
	free_exit(&info, NULL);
	return (0);
}
