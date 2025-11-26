/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_mapping.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 20:36:18 by piyu              #+#    #+#             */
/*   Updated: 2025/11/26 04:03:36 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

#ifndef BONUS
#else
/* Convert hit point coordinates to pixel location on the texture or normal map */
inline void	sphere_xyz_to_px_loc(t_vec p, t_object *sphere, int *i, int *j)
{
	double	phi;
	double	theta;

	p = add(p, sphere->oc);
	phi = atan2(p.z, p.x) + M_PI;
	theta = acos(-p.y / sphere->r);
	*i = phi / (2.0 * M_PI) * (sphere->texture->width - 1); // can also test if floor() looks better
	*j = theta / M_PI * (sphere->texture->height - 1);
}

/* Map pixel location to corresponding color value (r, g, b) */
inline t_color	px_loc_to_color(mlx_texture_t *map, int i, int j)
{
	int		idx;
	t_color	c;

	idx = (map->height - j - 1) * map->width + i;
	idx = idx * map->bytes_per_pixel;
	c.r = map->pixels[idx];
	c.g = map->pixels[idx + 1];
	c.b = map->pixels[idx + 2];
	return (c);
}

/* Parse the texture and normal maps for each object, could be inserted into parser later idk*/
inline void	parse_texture(t_object *obj, char *name)
{
	int		len;

	if (obj->type != SPHERE)
		return ;
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
	obj->normal = mlx_load_png(obj->normal_file);
	if (!obj->texture || !obj->normal)
		exit(free_exit("Loading texture failed", 1));


	exit(free_exit("Stop after loading texture", 1));
}
#endif
