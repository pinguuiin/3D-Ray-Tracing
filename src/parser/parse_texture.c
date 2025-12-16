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

static int	parse_texture_name(char **str, t_object *sphere, size_t line_num);
static int	allocate_texture_file_names(t_object *sphere, size_t len);
static int	load_textures(t_object *sphere, char *tex_name, size_t len);

/*
 * older, deprecated version?
int	parse_texture_for_sphere(char **str, t_object *sphere, size_t line_num)
{
	size_t	len;

	if (parse_and_normalize_vector(str, &sphere->axis, line_num, SPHERE_AXIS))
		return (INVALID_INPUT);

	//  *WARN: delete this when ready. it is going to require reviewing of all .rt files
	//  with textures to be within -1 to +1 range!!!
	// if (parse_3d_vector(str, &sphere->axis, line_num) == -1)
	// 	return (INVALID_INPUT);
	// if (!is_valid_tail_when_expecting_more_data(str, line_num))
	// 	return (INVALID_INPUT);
	// if (!validate_vector(&sphere->axis, line_num, SPHERE_AXIS))
	// 	return (INVALID_INPUT);

	skip_whitespace_but_not_newline(str);
	if (!**str || **str == '\n')
	{
		display_parsing_error("Unexpected texture input for sphere. If you'd "
			"like a sphere to be rendered with a texture,\nplease provide "
			"a valid axis vector for it, followed by the texture's .png file "
			"name (without the extension).\nError on line:", line_num);
		return (INVALID_INPUT);
	}
	// FIXME: adapt 
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
*/

int	parse_texture_for_sphere(char **str, t_object *sphere, size_t line_num)
{
	int	retval;

	if (parse_and_normalize_vector(str, &sphere->axis, line_num, SPHERE_AXIS))
		return (INVALID_INPUT);

	/*
	 *WARN: delete this when ready. it is going to require reviewing of all .rt files
	 with textures to be within -1 to +1 range!!!
	if (parse_3d_vector(str, &sphere->axis, line_num) == -1)
		return (INVALID_INPUT);
	if (!is_valid_tail_when_expecting_more_data(str, line_num))
		return (INVALID_INPUT);
	if (!validate_vector(&sphere->axis, line_num, SPHERE_AXIS))
		return (INVALID_INPUT);
	*/
	skip_whitespace_but_not_newline(str);
	if (!**str || **str == '\n')
	{
		display_parsing_error("Unexpected texture input for sphere. If you'd "
			"like a sphere to be rendered with a texture,\nplease provide "
			"a valid axis vector for it, followed by the texture's .png file "
			"name, without the extension and within double quotes.\n"
			"Error on line:", line_num);
		return (INVALID_INPUT);
	}

	/*
	if (**str != '\"')
	{
		display_parsing_error("Texture name has to be delimited by double "
			"quotes\nError on line:", line_num);
		return (INVALID_INPUT);
	}
	(*str)++; // move one beyond the initial quote
	len = strlen_texture_name(*str);
	if (allocate_texture_file_names(sphere, len) == -1)
		return (ALLOCATION_FAILURE);
	prepare_tex_names(sphere, *str, len);
	if (load_textures_and_free_them_upon_failure(sphere) == -1)
		return (LOAD_TEXTURE_FAIL);
	*/

	retval = parse_texture_name(str, sphere, line_num);
	if (retval)
		return (retval);

	get_object_rot_matrix(sphere->rot, sphere->axis);
	return (0);
	// *str += len;
	// return (parse_texture_name(str, sphere, line_num));
}

static int	parse_texture_name(char **str, t_object *sphere, size_t line_num)
{
	size_t	len;

	if (**str != '\"')
	{
		display_parsing_error("Texture name has to be delimited by double "
			"quotes\nError on line:", line_num);
		return (INVALID_INPUT);
	}
	(*str)++; // move one beyond the initial quote
	len = 0;
	while ((*str)[len] && (*str)[len] != '\"')
		len++;
	if ((*str)[len] != '\"')
	{
		display_parsing_error("Texture name has to be delimited by double "
			"quotes\nError on line:", line_num);
		return (INVALID_INPUT);
	}
	if (allocate_texture_file_names(sphere, len) == -1)
		return (ALLOCATION_FAILURE);
	// prepare_tex_names(sphere, *str, len);
	if (load_textures(sphere, *str, len) == -1)
		return (LOAD_TEXTURE_FAIL);
	*str += len + 1;
	return (0);
}



/*
* The length returned will always be greater than 0, since before calling this
* function, there is a check for a newline and a null terminator. This is
* important since that length will be used in order to allocate heap memory.
*/
/* WARN: deprecated version, without quote delimiters?
static size_t	strlen_texture_name(char *s)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != '\n')
		i++;
	return (i);
}
*/

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

/*
 * WARN: deprecated since included into load_textures()?
static void	prepare_tex_names(t_object *sphere, char *tex_name, size_t len)
{
	ft_memmove(sphere->tex_file, "./textures/", 11);
	ft_memmove(sphere->tex_file + 11, tex_name, len);
	ft_memmove(sphere->tex_file + 11 + len, "_color.png", 10);
	ft_memmove(sphere->normal_file, "./textures/", 11);
	ft_memmove(sphere->normal_file + 11, tex_name, len);
	ft_memmove(sphere->normal_file + 11 + len, "_normal.png", 11);
}
*/

/*
* Loads a texture via the MLX library, from both the color.png and normal.png
* versions of the texture. If a normal.png file is not available for the,
* texture, mlx_load_png() fails, but rendering still occurs, as miniRT handles
* it gracefully.
*/
static int	load_textures(t_object *sphere, char *tex_name, size_t len)
{
	ft_memmove(sphere->tex_file, "./textures/", 11);
	ft_memmove(sphere->tex_file + 11, tex_name, len);
	ft_memmove(sphere->tex_file + 11 + len, "_color.png", 10);
	ft_memmove(sphere->normal_file, "./textures/", 11);
	ft_memmove(sphere->normal_file + 11, tex_name, len);
	ft_memmove(sphere->normal_file + 11 + len, "_normal.png", 11);
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
	return (0);
}
#endif
