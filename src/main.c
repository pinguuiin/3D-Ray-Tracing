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

// return values:
//	1:	MLX function failures (WARN: not sure about this one!)
//	2:	invalid input - unexpected argument or misconfigured .rt file
//	3:	fatal system error during parsing (open(), malloc(), read(), empty
//	buffer for get_next_line_minirt())
// 	0:	If program ran smoothly
int	main(int argc, char *argv[])
{
	t_info	*info;

	// info is a static variable within get_info(), so its values are zeroed.
	info = get_info();

	if (parse_argument(argc, argv) == -1)
		return (2);
	parse_scene(info, argv[1]);

	// graphic rendering
	initialize_mlx(&info);
	mlx_key_hook(info.mlx, &key_handler, &info);
	mlx_loop_hook(info.mlx, draw, &info);
	mlx_loop(info.mlx);

	free_exit(NULL); // this does not actually exit the program, no worries
	return (0);
}
