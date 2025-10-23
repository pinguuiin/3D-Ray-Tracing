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

// return values:
//	1:	MLX function failures (WARN: not sure about this one!)
//	2:	invalid input from .rt file
//	3:	fatal system error during parsing (open(), malloc(), read(), empty
//	buffer for get_next_line_minirt())
// 	0:	If program ran smoothly
int	main(int argc, char *argv[])
{
	t_info	*info;

	// info is a static variable within get_info(), so its values are zeroed.
	info = get_info();

	if (argc == 2)
	{
		// FIXME: write a function that parses through the file's name, making
		// sure that it is a filename with no strange tails, that it ends
		// with ".rt" (!!!) and nothing else behind, except, if you want to be
		// very forgiving : accept whitespace, if the user would provide the
		// argument within quotes........
		// parse_scene() opens the whole argv[1] - consider cleaning up that
		// file name into a different string, if necessary, and pass that string
		// instead, allowing it to be freed from there as soon as the file is closed
		// AND every time you call free_exit() or clean_up_parsing_and_exit().
		// NOTE: Alternatively: can you simply overwrite the beginning of argv[1],
		// directly, null terminating it yourself, thus avoiding all trouble with
		// malloc() and free()???
		parse_scene(info, argv[1]);
	}
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

	free_exit(NULL); // this does not actually exit the program, no worries
	return (0);
}

static void	handle_unexpected_arg_count(int argc)
{
	if (argc == 1)
	{
		write(2, "No input scene file provided.\nPlease provide valid "
			".rt file for the miniRT program to render - for example:\n"
			"./miniRT beautiful_scene.rt\n",
			sizeof ("No input scene file provided.\nPlease provide valid "
			".rt file for the miniRT program to render - for example:\n"
			"./miniRT beautiful_scene.rt\n") - 1);
	}
	else
	{
		write(2, "Too many arguments provided. Please provide only one "
			"valid .rt scene\n",
			sizeof ("Too many arguments provided. Please provide only one "
			"valid .rt scene\n") - 1);
	}
}
