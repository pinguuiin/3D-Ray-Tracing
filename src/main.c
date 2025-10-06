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

int	main(void)
{
	t_info	info;

	// TODO: initialize info struct! At least the parsing elements.



	// graphic rendering
	initialize_mlx(&info);
	mlx_key_hook(info.mlx, &key_handler, &info);
	mlx_loop_hook(info.mlx, draw, &info);
	mlx_loop(info.mlx);

	// free memory, and return (0)
	// WARN: change this to only free the memory before returning 0.
	free_exit(&info, NULL);
	return (0);
}
