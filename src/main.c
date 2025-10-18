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

static void	handle_unexpected_arg_count(int argc);

int	main(int argc, char *argv[])
{
	t_info	*info;

	// info is a static variable within get_info(), so its values are zeroed.
	info = get_info();

	if (argc == 2)
		parse_scene(info, argv[1]);
	else
	{
		handle_inappropriate_argc(argc);
		return (1);
	}

	// graphic rendering
	initialize_mlx(&info);
	mlx_key_hook(info.mlx, &key_handler, &info);
	mlx_loop_hook(info.mlx, draw, &info);
	mlx_loop(info.mlx);

	// free memory, and return (0)
	// WARN: change this to only free all the memory before returning 0
	// (free_exit() exits with 1!)
	free_exit(NULL);
	return (0);
}

static void	handle_unexpected_arg_count(int argc)
{
	if (argc == 1)
		write(2, "No input scene file provided.\nPlease provide valid "
			".rt file for the miniRT program to render - for example:\n"
			"./miniRT beautiful_scene.rt\n",
			sizeof ("No input scene file provided.\nPlease provide valid "
			".rt file for the miniRT program to render - for example:\n"
			"./miniRT beautiful_scene.rt\n") - 1);
	else
		write(2, "Too many arguments provided. Please provide only one "
			"valid .rt scene\n",
			sizeof ("Too many arguments provided. Please provide only one "
			"valid .rt scene\n") - 1);
}
