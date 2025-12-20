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

static int	parse_texture_name(char **str, t_object *object, size_t line_num);
static int	allocate_texture_file_names(t_object *object, size_t len);
static int	load_textures(t_object *object, char *tex_name, size_t len);

int	parse_texture(char **str, t_object *object, size_t line_num)
{
	int	retval;

	if (object->type == SPHERE)
	{
		if (parse_and_normalize_vector(str, &object->axis, line_num,
				SPHERE_AXIS) == -1)
			return (INVALID_INPUT);
		skip_whitespace_but_not_newline(str);
		if (!**str || **str == '\n')
		{
			display_parsing_error("Unexpected texture input for sphere. If "
				"you'd like a sphere to be rendered with a texture,\n"
				"please provide a valid axis vector for it, followed by the "
				"texture's .png file name, without the extension and within "
				"double quotes.\nError on line:", line_num);
			return (INVALID_INPUT);
		}
	}
	retval = parse_texture_name(str, object, line_num);
	if (retval)
		return (retval);
	get_object_rot_matrix(object->rot, object->axis);
	return (0);
}

static int	parse_texture_name(char **str, t_object *object, size_t line_num)
{
	size_t	len;

	if (**str != '\"')
	{
		display_parsing_error("Texture name has to be delimited by double "
			"quotes.\nError on line:", line_num);
		return (INVALID_INPUT);
	}
	(*str)++;
	len = 0;
	while ((*str)[len] && (*str)[len] != '\"')
		len++;
	if ((*str)[len] != '\"')
	{
		display_parsing_error("Texture name has to be delimited by double "
			"quotes.\nError on line:", line_num);
		return (INVALID_INPUT);
	}
	if (allocate_texture_file_names(object, len) == -1)
		return (ALLOCATION_FAILURE);
	if (load_textures(object, *str, len) == -1)
		return (LOAD_TEXTURE_FAIL);
	*str += len + 1;
	return (0);
}

static int	allocate_texture_file_names(t_object *object, size_t len)
{
	object->tex_file = ft_calloc(len + 22, sizeof (char));
	if (!object->tex_file)
		return (-1);
	object->normal_file = ft_calloc(len + 23, sizeof (char));
	if (!object->normal_file)
	{
		free(object->tex_file);
		object->tex_file = NULL;
		return (-1);
	}
	return (0);
}

/*
* Loads a texture via the MLX library, from both the color.png and normal.png
* versions of the texture. If a normal.png file is not available for the,
* texture, mlx_load_png() fails, but rendering still occurs, as miniRT handles
* it gracefully.
*/
static int	load_textures(t_object *object, char *tex_name, size_t len)
{
	ft_memmove(object->tex_file, "./textures/", 11);
	ft_memmove(object->tex_file + 11, tex_name, len);
	ft_memmove(object->tex_file + 11 + len, "_color.png", 10);
	ft_memmove(object->normal_file, "./textures/", 11);
	ft_memmove(object->normal_file + 11, tex_name, len);
	ft_memmove(object->normal_file + 11 + len, "_normal.png", 11);
	object->texture = mlx_load_png(object->tex_file);
	if (!object->texture)
	{
		free(object->tex_file);
		free(object->normal_file);
		object->tex_file = NULL;
		object->normal_file = NULL;
		ft_putstr_fd("Loading texture map failed. Aborting miniRT.\n", 2);
		return (-1);
	}
	object->normal = mlx_load_png(object->normal_file);
	return (0);
}
#endif
