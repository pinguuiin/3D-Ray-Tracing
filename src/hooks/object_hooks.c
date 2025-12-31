/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 22:36:08 by piyu              #+#    #+#             */
/*   Updated: 2025/12/31 06:55:42 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

#ifndef BONUS
#else

inline void	rotate_object(mlx_key_data_t keydata, t_info *info)
{
	if (keydata.key == MLX_KEY_1)
		info->auto_rotate = true;
	else
		info->auto_rotate = false;
}

static inline void	update_obj_oc_and_plane_normal(t_info *info, t_object *obj)
{
	obj->oc = subtract(info->cam_curr_frame.pos, obj->pos);
	if (obj->type == PLANE && dot(obj->axis, obj->oc) < 0)
		obj->axis = scale(obj->axis, -1);
}

inline void	move_selected_object(t_info *info)
{
	int32_t			cursor[2];
	const	double	(*rot)[3] = info->rot;
	double			depth;
	t_object		*obj;

	obj = info->selected_obj;
	if (obj)
	{
		mlx_get_mouse_pos(info->mlx, &cursor[0], &cursor[1]);
		if (info->prev_mouse == true)
		{
			depth = fabs(dot(obj->oc, vec3(rot[0][2], rot[1][2], rot[2][2])))
				* info->px;
			obj->pos = add(obj->pos, scale(vec3(rot[0][0], rot[1][0],
							rot[2][0]), (cursor[0] - info->prev_x) * depth));
			obj->pos = subtract(obj->pos, scale(vec3(rot[0][1], rot[1][1],
							rot[2][1]), (cursor[1] - info->prev_y) * depth));
			update_obj_oc_and_plane_normal(info, obj);
		}
		info->prev_x = cursor[0];
		info->prev_y = cursor[1];
		info->prev_mouse = true;
	}
	else
		info->prev_mouse = false;
}

inline void	mouse_hook(mouse_key_t button, action_t action,
	modifier_key_t mods, void *param)
{
	t_info	*info;
	int32_t	cursor[2];
	t_vec	ray;
	t_hit	hit;

	(void)mods;
	info = (t_info *)param;
	if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS)
	{
		mlx_get_mouse_pos(info->mlx, &cursor[0], &cursor[1]);
		ray = vec3(cursor[0] * info->px - info->viewport_w / 2.0,
				-(cursor[1] * info->px - info->viewport_h / 2.0), 0);
		rotate(info->rot, &ray);
		ray = normalize(add(info->cam_curr_frame.direction, ray));
		hit.emit_pos = info->cam_curr_frame.pos;
		if (nearest_ray_hit(info, ray, hit.emit_pos, &hit) + 1.0 > EPSILON)
			info->selected_obj = &info->obj[hit.obj_id];
	}
	else if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_RELEASE)
		info->selected_obj = NULL;
}
#endif
