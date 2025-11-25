/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_mapping.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 20:36:18 by piyu              #+#    #+#             */
/*   Updated: 2025/11/25 22:06:21 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

#ifndef BONUS
/* Define u as the horizontal coordinate in the range of [0, 1],
v as the vertical coordinate in the range of [0, 1] in the reference frame
of a unit sphere from the origin */
t_vec	uv_to_xyz(double u, double v, double r)
{
	double	phi;
	double	theta;
	t_vec	vec;

	phi = u * 2.0 * M_PI;
	theta = v * M_PI;
	vec.x = - cos(phi) * sin(theta);
	vec.y = - cos(theta);
	vec.z = sin(phi) * sin(theta);
	return (vec);
}

t_vec	img_to_xyz(t_object *sphere, int i, int nx, int ny)
{
	double	u;
	double	v;

	u = (double)(i % nx) / (double)(nx - 1);
	v = 1.0 - (double)(i / nx) / (double)(ny - 1);
	return (uv_to_xyz(u, v, sphere->r));
}

/* Parse the texture and normal maps for each object, could be inserted into parser later idk*/
void	parse_texture(t_object *obj, char *name)
{
	int		len;

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
}

#endif
