/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 15:59:24 by ykadosh           #+#    #+#             */
/*   Updated: 2025/10/01 19:01:12 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_info	*get_info(void)
{
	static t_info	info;

	return (&info);
}

int	free_exit(char *s)
{
	t_info	*info;

	info = get_info();
	free(info->light);
	free(info->obj);
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
		exit(free_exit("Instance initialization failed"));

	info->img = mlx_new_image(info->mlx, WIDTH, HEIGHT);
	if (!info->img)
		exit(free_exit("Image buffer creation failed"));
	if (mlx_image_to_window(info->mlx, info->img, 0, 0) == -1)
		exit(free_exit("Pushing image to window failed"));
}

/*
* Return Values:
* 1: MLX function failures (WARN: not sure about this one!)
* 2: invalid input - unexpected argument or misconfigured .rt file
* 3: fatal system error during parsing, such as failures of open(), malloc(),
* 		read() and close() functions, or buffer for get_next_line_revised() is
*		predefined as empty.
* 0: If program runs smoothly
*/
int	main(int argc, char *argv[])
{
	t_info	*info;

	info = get_info();

	parse_argument(argc, argv);
	parse_scene(info, argv[1]);

	// graphic rendering
	initialize_mlx(&info);
	mlx_key_hook(info.mlx, &key_handler, &info);
	mlx_loop_hook(info.mlx, draw, &info);
	mlx_loop(info.mlx);

	free_exit(NULL); // this does not actually exit the program, no worries
	return (SUCCESS);
}
