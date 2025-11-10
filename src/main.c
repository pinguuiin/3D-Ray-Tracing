/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 15:59:24 by ykadosh           #+#    #+#             */
/*   Updated: 2025/11/17 00:38:47 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minirt.h"

t_info	*get_info(void)
{
	static t_info	info;

	return (&info);
}

int	free_exit(char *s, int exit_code)
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
	return (exit_code);
}

static void	key_handler(mlx_key_data_t keydata, void *param)
{
	t_info	*info;

	info = (t_info *)param;
	if (keydata.key == MLX_KEY_ESCAPE)
	{
		info->exit_flag = 1;
		// WARN: perhaps these are better off done from renderer(),
		// now that we have threads to join, a mutex to destroy..... and we
		// probably do not want the threads still working AFTER the window has
		// closed... First let them finish, then destroy the mutex, and only
		// then close the window....
		//
		// destroy_threads(info, N_THREADS);
		// mlx_close_window(info->mlx);
	}
	else if (keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_A
		|| keydata.key == MLX_KEY_Q || keydata.key == MLX_KEY_Z
		|| keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_S)
		move_camera(keydata, info);
	else if (keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_DOWN
		|| keydata.key == MLX_KEY_RIGHT || keydata.key == MLX_KEY_LEFT)
		rotate_camera(keydata, info);
}

void	initialize_mlx(t_info *info)
{
	info->mlx = mlx_init(WIDTH, HEIGHT, "miniRT", true);
	if (!info->mlx)
		exit(free_exit("Instance initialization failed", MLX_FAILURE));

	info->img = mlx_new_image(info->mlx, WIDTH, HEIGHT);
	if (!info->img)
		exit(free_exit("Image buffer creation failed", MLX_FAILURE));
	if (mlx_image_to_window(info->mlx, info->img, 0, 0) == -1)
		exit(free_exit("Pushing image to window failed", MLX_FAILURE));
}


/*
* Return Values:
* 1: MLX function failures (WARN: not sure about this one!)
* 2: invalid input - unexpected argument or misconfigured .rt file
* 3: fatal system error during parsing, such as failures of open(), malloc(),
* 		read(), close() and pthread_create() functions, or buffer for
* 		get_next_line_revised() is predefined as empty.
* 0: If program runs smoothly
*/
int	main(int argc, char *argv[])
{
	t_info	*info;

	info = get_info();

	parse_argument(argc, argv);
	parse_scene(info, argv[1]);

	preprocessor(info);

	init_threads(info);

	initialize_mlx(info);
	mlx_resize_hook(info->mlx, &resize, info);
	mlx_key_hook(info->mlx, &key_handler, info);
	mlx_loop_hook(info->mlx, renderer, info);
	mlx_loop(info->mlx);

	(void)free_exit(NULL, 0); // this does not actually exit the program, no worries
	return (SUCCESS);
}
