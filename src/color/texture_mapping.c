/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_mapping.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 20:36:18 by piyu              #+#    #+#             */
/*   Updated: 2025/12/03 08:22:20 by piyu             ###   ########.fr       */
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

	p = subtract(p, sphere->pos);
	rotate(sphere->rot, &p);
	phi = fmod(atan2(p.z, p.x) + M_PI + sphere->phase, 2 * M_PI);
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
	
	get_rotation_matrix(rot, scale(normalize(hit->op), -1), obj->axis);
	hit->normal.z = -hit->normal.z;
	rotate(rot, &hit->normal);
}

/* Parse the texture and normal maps for each object, could be inserted into parser later idk*/
inline void	parse_texture(t_object *obj, char *name)
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
	if (!obj->normal)
	{
		mlx_delete_texture(obj->texture);
		exit(free_exit("Loading normal map failed", 1));
	}
	obj->has_tex = true;
	if (obj->type == SPHERE)
		obj->axis = normalize(vec3(0.43, 1.0, 0.0));
	get_object_rot_matrix(obj->rot, obj->axis);
}
#endif
