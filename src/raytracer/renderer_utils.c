/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 05:33:21 by piyu              #+#    #+#             */
/*   Updated: 2025/12/31 06:25:14 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

#ifndef BONUS
#else

inline void	add_diffuse_and_specular(t_object *obj, t_hit *hit, t_light *light)
{
	double	flux;
	double	spec;

	if (obj->material == ICE)
		hit->f = obj->ks + (1.0 - obj->ks)
			* pow(1.0 - dot(hit->normal, hit->ray), 5.0);
	flux = dot(hit->incoming, hit->normal);
	if (flux > EPSILON)
	{
		flux *= obj->kd;
		if (obj->material == ICE)
			flux *= 1.0 - hit->f;
		hit->diffuse = scale(dot_elem(light->color, hit->color), flux);
		hit->intensity = add(hit->intensity, hit->diffuse);
		spec = dot(hit->outgoing, hit->ray);
		if (spec > EPSILON)
		{
			spec = hit->f * pow(spec, obj->shininess);
			hit->specular = scale(light->color, spec);
			hit->intensity = add(hit->intensity, hit->specular);
		}
	}
}
#endif

static inline void	update_camera_for_new_frame(t_info *info)
{
	if (info->has_rotated)
	{
		info->has_rotated = 0;
		info->cam_curr_frame.direction = info->cam.direction;
		get_rotation_matrix(info->rot, info->cam_curr_frame.direction,
			vec3(0, 1, 0));
	}
	if (info->has_moved)
	{
		info->has_moved = 0;
		info->cam_curr_frame.pos = info->cam.pos;
		update_oc_and_plane_normal(info);
	}
}

#ifndef BONUS

void	update_data_for_new_frame(t_info *info)
{
	update_camera_for_new_frame(info);
	if ((uint32_t) info->mlx->height != info->img->height
		|| (uint32_t)info->mlx->width != info->img->width)
		resize(info->mlx->width, info->mlx->height, info);
}
#else

static inline void	update_sphere_phase_for_new_frame(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->n_obj)
	{
		if (info->obj[i].type == SPHERE)
			info->obj[i].phase -= 0.3 * info->mlx->delta_time;
		i++;
	}
}

void	update_data_for_new_frame(t_info *info)
{
	int	i;

	update_camera_for_new_frame(info);
	if (info->auto_rotate == true)
		update_sphere_phase_for_new_frame(info);
	move_selected_object(info);
	if ((uint32_t) info->mlx->height != info->img->height
		|| (uint32_t)info->mlx->width != info->img->width)
	{
		resize(info->mlx->width, info->mlx->height, info);
		if (atomic_load(&info->thread_system.is_multithreaded))
		{
			i = 0;
			while (i < N_THREADS)
			{
				init_chunk_borders(info->img->width,
					&info->thread_system.threads[i], i);
				i++;
			}
		}
	}
}
#endif
