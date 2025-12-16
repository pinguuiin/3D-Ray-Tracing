/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocessor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 04:45:19 by piyu              #+#    #+#             */
/*   Updated: 2025/12/16 07:16:31 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

inline void	update_oc_and_plane_normal(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->n_obj)
	{
		info->obj[i].oc = subtract(info->cam_curr_frame.pos, info->obj[i].pos);
		if (info->obj[i].type == PLANE
			&& dot(info->obj[i].axis, info->obj[i].oc) < 0)
			info->obj[i].axis = scale(info->obj[i].axis, -1);
		i++;
	}
}

inline void	get_viewport_data(t_info *info)
{
	info->focal_length = 1.0;
	info->viewport_w = tan(info->cam_curr_frame.fov / 2.0)
		* 2 * info->focal_length;
	info->viewport_h = info->viewport_w * info->img->height / info->img->width;
	info->px = info->viewport_w / info->img->width;
}

#ifndef BONUS

void	preprocessor(t_info *info)
{
	info->ray_depth = 2;
	info->cam_curr_frame = info->cam;
	update_oc_and_plane_normal(info);
	get_rotation_matrix(info->rot, info->cam_curr_frame.direction,
		vec3(0, 1, 0));
	get_viewport_data(info);
}


/* ==========================to be deleted=========================== */
#else
static inline void	parse_plane_texture(t_object *obj, char *name)
{
	int	len;

	len = ft_strlen(name);
	obj->tex_file = calloc(len + 22, sizeof(char));
	obj->normal_file = calloc(len + 23, sizeof(char));
	if (!obj->tex_file || !obj->normal_file)
		exit(free_exit("Malloc failed", 1));
	ft_memmove(obj->tex_file, "./textures/", 11);
	ft_memmove(obj->normal_file, "./textures/", 11);
	ft_memmove(obj->tex_file + 11, name, len);
	ft_memmove(obj->normal_file + 11, name, len);
	ft_memmove(obj->tex_file + 11 + len, "_color.png", 10);
	ft_memmove(obj->normal_file + 11 + len, "_normal.png", 11);
	obj->texture = mlx_load_png(obj->tex_file);
	if (!obj->texture)
		exit(free_exit("Loading texture map failed", 1));
	obj->normal = mlx_load_png(obj->normal_file);
	get_object_rot_matrix(obj->rot, obj->axis);
}

void	preprocessor(t_info *info)
{
	int	i;

	info->ray_depth = 2;
	info->cam_curr_frame = info->cam;
	update_oc_and_plane_normal(info);
	get_rotation_matrix(info->rot, info->cam_curr_frame.direction,
		vec3(0, 1, 0));
	get_viewport_data(info);
	i = 0;
	while (i < info->n_obj)
	{
		if (info->obj[i].type == PLANE)
			parse_plane_texture(&info->obj[i], "earth");
		i++;
	}
}
#endif


