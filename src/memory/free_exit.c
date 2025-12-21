/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 20:11:47 by ykadosh           #+#    #+#             */
/*   Updated: 2025/12/20 20:12:50 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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

static void	free_all_textures(t_info *info)
{
	int	i;

	i = 0;
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
}

int	free_exit(char *s, int exit_code)
{
	t_info	*info;

	info = get_info();
	if (atomic_load(&info->thread_system.is_multithreaded))
	{
		atomic_store(&info->thread_system.exit_flag, 1);
		pthread_barrier_wait(&info->thread_system.barrier);
		clean_up_threads_and_barrier(&info->thread_system, N_THREADS);
	}
	free_all_textures(info);
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
