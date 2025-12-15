/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_mapping.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 20:36:18 by piyu              #+#    #+#             */
/*   Updated: 2025/12/15 02:52:29 by piyu             ###   ########.fr       */
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
	*i = phi / (2.0 * M_PI) * (sphere->texture->width - 1);
	*j = theta / M_PI * (sphere->texture->height - 1);
}

/* Map pixel location to corresponding color value (r, g, b) */
inline t_color	px_loc_to_color(mlx_texture_t *map, int i, int j)
{
	int		idx;
	t_color	c;

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
inline void	sphere_tbn_to_xyz(t_object *obj, t_hit *hit)
{
	double	rot[3][3];
	t_vec	geo_normal;

	geo_normal = normalize(hit->op);
	get_rotation_matrix(rot, normalize(scale(hit->op, -1)), obj->axis);
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
