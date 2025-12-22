/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 22:21:42 by ykadosh           #+#    #+#             */
/*   Updated: 2025/12/22 08:00:07 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#ifndef BONUS
#else

static int	parse_texture_name(char **str, t_object *object, size_t line_num);
static int	count_texture_strlen(char *str, size_t line_num, size_t *len);
static int	assign_material_param(t_object *object, size_t len, char *str);
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
	if (object->material != MONO)
		get_object_rot_matrix(object->rot, object->axis);
	return (0);
}

static int	parse_texture_name(char **str, t_object *object, size_t line_num)
{
	size_t	len;
	int		retval;

	if (**str != '\"')
	{
		display_parsing_error("Texture name has to be delimited by double "
			"quotes.\nError on line:", line_num);
		return (INVALID_INPUT);
	}
	(*str)++;
	if (count_texture_strlen(*str, line_num, &len) == -1)
		return (INVALID_INPUT);
	if (assign_material_param(object, len, *str))
	{
		object->material = TEXTURE;
		retval = load_textures(object, *str, len);
		if (retval == -2)
			return (ALLOCATION_FAILURE);
		else if (retval == -1)
			return (LOAD_TEXTURE_FAIL);
	}
	*str += len + 1;
	return (0);
}

static int	count_texture_strlen(char *str, size_t line_num, size_t *len)
{
	size_t	i;

	i = 0;
	while (str[i] && str[i] != '\"')
		i++;
	if (str[i] != '\"')
	{
		display_parsing_error("Texture name has to be delimited by double "
			"quotes.\nError on line:", line_num);
		return (-1);
	}
	*len = i;
	return (0);
}

static int	assign_material_param(t_object *object, size_t len, char *str)
{
	if (len == 7 && !ft_strncmp(str, "checker", 7))
	{
		object->material = CHECKER;
		return (0);
	}
	if (len == 5 && !ft_strncmp(str, "metal", 5))
	{
		object->ks = 0.9;
		object->kd = 0.4;
		object->shininess = 80;
	}
	else if (len == 3 && !ft_strncmp(str, "ice", 3))
	{
		object->ks = 0.5;
		object->kd = 0.5;
		object->shininess = 30;
	}
	else if (len == 4 && !ft_strncmp(str, "snow", 4))
	{
		object->ks = 0.02;
		object->kd = 0.8;
		object->shininess = 10;
	}
	else
	{
		object->ks = 0.2;
		object->kd = 0.8;
	}
	return (1);
}

/*
* Loads a texture via the MLX library, from both the color.png and normal.png
* versions of the texture. If a normal.png file is not available for the,
* texture, mlx_load_png() fails, but rendering still occurs, as miniRT handles
* it gracefully.
*/
static int	load_textures(t_object *object, char *tex_name, size_t len)
{
	object->tex_file = ft_calloc(len + 22, sizeof (char));
	object->normal_file = ft_calloc(len + 23, sizeof (char));
	if (!object->tex_file || !object->normal_file)
		return (-2);
	ft_memmove(object->tex_file, "./textures/", 11);
	ft_memmove(object->tex_file + 11, tex_name, len);
	ft_memmove(object->tex_file + 11 + len, "_color.png", 10);
	ft_memmove(object->normal_file, "./textures/", 11);
	ft_memmove(object->normal_file + 11, tex_name, len);
	ft_memmove(object->normal_file + 11 + len, "_normal.png", 11);
	object->texture = mlx_load_png(object->tex_file);
	if (!object->texture)
	{
		ft_putstr_fd("Loading texture map failed. Aborting miniRT.\n", 2);
		return (-1);
	}
	object->normal = mlx_load_png(object->normal_file);
	return (0);
}
#endif
