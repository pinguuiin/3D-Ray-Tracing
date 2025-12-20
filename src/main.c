/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 15:59:24 by ykadosh           #+#    #+#             */
/*   Updated: 2025/12/16 00:34:05 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minirt.h"

t_info	*get_info(void)
{
	static t_info	info;

	return (&info);
}

#ifndef BONUS

int	free_exit(char *s, int exit_code)
{
	t_info	*info;

	info = get_info();
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
#else

int	free_exit(char *s, int exit_code)
{
	int		i;
	t_info	*info;

	i = 0;
	info = get_info();
	if (atomic_load(&info->thread_system.is_multithreaded))
	{
		atomic_store(&info->thread_system.exit_flag, 1);
		pthread_barrier_wait(&info->thread_system.barrier);
		clean_up_threads_and_barrier(&info->thread_system, N_THREADS);
	}
	while (i < info->n_obj)
	{
		if (info->obj[i].tex_file)
		{
			free(info->obj[i].tex_file);
			free(info->obj[i].normal_file);
			mlx_delete_texture(info->obj[i].texture);
			if (info->obj[i].normal)
				mlx_delete_texture(info->obj[i].normal);
		}
		i++;
	}
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
#endif

void	initialize_mlx(t_info *info)
{
	info->mlx = mlx_init(WIDTH, HEIGHT, "miniRT", true);
	if (!info->mlx)
		exit(free_exit("Instance initialization failed", MLX_FAILURE));
	mlx_set_window_limit(info->mlx, 50, 50, -1, -1);
	info->img = mlx_new_image(info->mlx, WIDTH, HEIGHT);
	if (!info->img)
		exit(free_exit("Image buffer creation failed", MLX_FAILURE));
	if (mlx_image_to_window(info->mlx, info->img, 0, 0) == -1)
		exit(free_exit("Pushing image to window failed", MLX_FAILURE));
}

#ifndef BONUS

int	main(int argc, char *argv[])
{
	t_info	*info;

	info = get_info();
	parse_argument(argc, argv);
	parse_scene(info, argv[1]);
	initialize_mlx(info);
	preprocessor(info);
	mlx_key_hook(info->mlx, &key_handler, info);
	mlx_loop_hook(info->mlx, renderer, info);
	mlx_loop(info->mlx);
	(void)free_exit(NULL, 0);
	return (SUCCESS);
}
#else

int	main(int argc, char *argv[])
{
	t_info	*info;

	info = get_info();
	parse_argument(argc, argv);
	parse_scene(info, argv[1]);
	initialize_mlx(info);
	preprocessor(info);
	initialize_multithreading(info);
	mlx_key_hook(info->mlx, &key_handler, info);
	mlx_mouse_hook(info->mlx, mouse_hook, info);
	mlx_loop_hook(info->mlx, renderer, info);
	mlx_loop(info->mlx);
	(void)free_exit(NULL, 0);
	return (SUCCESS);
}
#endif
