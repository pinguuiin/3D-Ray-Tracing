/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 22:21:42 by ykadosh           #+#    #+#             */
/*   Updated: 2025/11/29 22:21:53 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#ifndef BONUS
#else

static size_t	strlen_texture_name(char *s);
static int		allocate_texture_file_names(t_object *sphere, size_t len);
static void		prepare_tex_names(t_object *sphere, char *tex_name, size_t len);
static int		load_textures_and_free_them_upon_failure(t_object *sphere);

int	parse_texture_for_sphere(char **str, t_object *sphere, size_t line_num)
{
	size_t	len;

	// parse axis vector
	if (parse_3d_vector(str, &sphere->axis, line_num) == -1)
		return (-1);

	// set the z value of the sphere's axis to 0, and normalize the vector.
	if (!validate_vector(&sphere->axis, line_num, SPHERE_AXIS))
		return (INVALID_INPUT);

	skip_whitespace_but_not_newline(str);

	if (!**str || **str == '\n')
	{
		display_parsing_error("Unexpected texture input for sphere. If you'd "
			"like a sphere to be rendered with a texture,\nplease provide "
			"a valid axis vector for it, followed by the texture's .png file "
			"name (without the extension).\nError on line:", line_num);
		return (INVALID_INPUT);
	}

	// parse string (texture file name)
	len = strlen_texture_name(*str);
	if (allocate_texture_file_names(sphere, len) == -1)
		return (ALLOCATION_FAILURE);
	prepare_tex_names(sphere, *str, len);

	if (load_textures_and_free_them_upon_failure(sphere) == -1)
		return (LOAD_TEXTURE_FAIL);

	get_object_rot_matrix(sphere->rot, sphere->axis);
	*str += len;
	return (0);
}

// this will always be greater than 0, since there is a check for null terminator
// and newline BEFORE calling it.
static size_t	strlen_texture_name(char *s)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != '\n')
		i++;
	return (i);
}

static int	allocate_texture_file_names(t_object *sphere, size_t len)
{
	sphere->tex_file = ft_calloc(len + 22, sizeof (char));
	if (!sphere->tex_file)
		return (-1);
	sphere->normal_file = ft_calloc(len + 23, sizeof (char));
	if (!sphere->normal_file)
	{
		free(sphere->tex_file);
		sphere->tex_file = NULL;
		return (-1);
	}
	return (0);
}

static void	prepare_tex_names(t_object *sphere, char *tex_name, size_t len)
{
	ft_memmove(sphere->tex_file, "./textures/", 11);
	ft_memmove(sphere->tex_file + 11, tex_name, len);
	ft_memmove(sphere->tex_file + 11 + len, "_color.png", 10);

	ft_memmove(sphere->normal_file, "./textures/", 11);
	ft_memmove(sphere->normal_file + 11, tex_name, len);
	ft_memmove(sphere->normal_file + 11 + len, "_normal.png", 11);
}

static int	load_textures_and_free_them_upon_failure(t_object *sphere)
{
	sphere->texture = mlx_load_png(sphere->tex_file);
	if (!sphere->texture)
	{
		free(sphere->tex_file);
		free(sphere->normal_file);
		sphere->tex_file = NULL;
		sphere->normal_file = NULL;
		ft_putstr_fd("Loading texture map failed. Aborting miniRT.\n", 2);
		return (-1);
	}
	sphere->normal = mlx_load_png(sphere->normal_file);
	// no need to handle the error, if loading the .png for sphere->normal has
	// failed after this call, it is okay and we still render without it.
	return (0);
}
#endif
