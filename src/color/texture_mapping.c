/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_mapping.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 20:36:18 by piyu              #+#    #+#             */
/*   Updated: 2025/12/21 06:37:35 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

#ifndef BONUS
#else
/* Convert hit point coordinates to pixel location on the texture map */
inline void	sphere_xyz_to_px_loc(t_vec p, t_object *sphere, int *i, int *j)
{
	double	phi;
	double	theta;

	p = subtract(p, sphere->pos);
	rotate(sphere->rot, &p);
	phi = fmod(atan2(p.z, p.x) + M_PI + sphere->phase, 2 * M_PI);
	theta = acos(-p.y / sphere->r);
	if (sphere->material == CHECKER)
	{
		*i = floor(phi / (2.0 * M_PI) * 16.0);
		*j = floor(theta / M_PI * 8.0);
		return ;
	}
	*i = phi / (2.0 * M_PI) * (sphere->texture->width - 1);
	*j = theta / M_PI * (sphere->texture->height - 1);
}

inline void	plane_xyz_to_px_loc(t_vec p, t_object *plane, int *i, int *j)
{
	t_info	*info;

	info = get_info();
	p = subtract(p, plane->pos);
	rotate(plane->rot, &p);
	if (plane->material == CHECKER)
	{
		*i = floor(p.x);
		*j = floor(p.z);
		return ;
	}
	*i = fmod(p.x / info->px, plane->texture->width - 1);
	*j = fmod(p.z / info->px, plane->texture->height - 1);
	if (*i < 0)
		*i += plane->texture->width;
	if (*j < 0)
		*j += plane->texture->height;
}

/* Map pixel location to corresponding color value (r, g, b) */
inline t_color	px_loc_to_color(t_object *obj, int i, int j)
{
	t_color			c;
	int				idx;
	mlx_texture_t	*map;

	if (obj->material == CHECKER)
	{
		if ((i + j) % 2 == 0)
			c = vec3(1.0, 1.0, 1.0);
		else
			c = vec3(0.0, 0.0, 0.0);
		return (c);
	}
	map = obj->texture;
	idx = (map->height - j - 1) * map->width + i;
	idx = idx * map->bytes_per_pixel;
	c.r = map->pixels[idx] / 255.0;
	c.g = map->pixels[idx + 1] / 255.0;
	c.b = map->pixels[idx + 2] / 255.0;
	return (c);
}

/* Map pixel location to corresponding normal vector in tangent space */
inline t_vec	px_loc_to_normal(mlx_texture_t *map, int i, int j)
{
	int		idx;
	t_vec	normal;

	idx = (map->height - j - 1) * map->width + i;
	idx = idx * map->bytes_per_pixel;
	normal.x = (map->pixels[idx] / 255.0 - 0.5) * 2.0;
	normal.y = (map->pixels[idx + 1] / 255.0 - 0.5) * 2.0;
	normal.z = (map->pixels[idx + 2] / 255.0 - 0.5) * 2.0;
	return (normalize(normal));
}

/* Convert normal map from tangent space to world space */
inline void	normal_tbn_to_xyz(t_object *obj, t_hit *hit)
{
	double	rot[3][3];
	t_vec	geo_normal;

	if (obj->type == SPHERE)
	{
		geo_normal = normalize(hit->op);
		get_rotation_matrix(rot, scale(geo_normal, -1), obj->axis);
	}
	else
	{
		geo_normal = obj->axis;
		get_rotation_matrix(rot, scale(geo_normal, -1), vec3(0.0, 1.0, 0.0));
	}
	hit->normal.z = -hit->normal.z;
	rotate(rot, &hit->normal);
	hit->normal = normalize(hit->normal);
	if (dot(geo_normal, subtract(hit->emit_pos, hit->pos)) < 0)
	{
		hit->normal = scale(hit->normal, -1);
		geo_normal = scale(geo_normal, -1);
	}
	hit->pos = add(hit->pos, scale(geo_normal, 0.0001));
}
#endif
