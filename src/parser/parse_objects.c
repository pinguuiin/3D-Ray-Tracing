/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 07:32:46 by ykadosh           #+#    #+#             */
/*   Updated: 2025/11/25 20:34:13 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	create_new_object_node(t_parser *parser);

#ifndef BONUS
#else
static void	free_all_textures_from_linked_list(t_node_obj *p_head);
#endif

// NOTE: In all functions of parse_setting.c and parse_objects.c:
// The pointer 'str' is always pointing one byte past the scene's type
// identifier ('A', 'C', 'L', "sp", "pl" or "cy") AND the whitespace
// (non-newline) character that follows it!

#ifndef BONUS
int	parse_sphere(t_parser *parser, char *str, size_t line_num)
{
	t_object	*sphere;

	sphere = NULL;

	// initialize sphere

	if (create_new_object_node(parser) == -1)
		return (ALLOCATION_FAILURE);

	sphere = &parser->current->object;

	sphere->type = SPHERE;


	// start parsing 'str'
	skip_whitespace_but_not_newline(&str);

	// parse x,y,z coordinates of the center of the sphere (t_vec 'pos')
	if (parse_3d_vector(&str, &sphere->pos, line_num) == -1)
		return (INVALID_INPUT);
	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (INVALID_INPUT);

	skip_whitespace_but_not_newline(&str);

	// parse the sphere's diameter, and convert it to radius (double 'r')
	if (ft_strtod(&str, &sphere->r, line_num) == -1)
		return (INVALID_INPUT);
	if (sphere->r < EPSILON)  // No need to use fabs() here, since a negative value does not make sense for the diameter.
	{
		display_parsing_error("Unable to render sphere: diameter provided "
			"has to be a positive value, not too close to zero. See line:", line_num);
		return (INVALID_INPUT);
	}
	sphere->r *= 0.5; // convert diameter to radius.

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);

	// parse R,G,B colors in range [0-255]
	if (parse_color(&str, &sphere->color, NULL, line_num) == -1)
		return (INVALID_INPUT);

	if (!is_valid_end_of_line(str, line_num))
		return (INVALID_INPUT);

	// check that not too many objects were provided by the user, before
	// incrementing their counter.
	if (!is_valid_n_elements(parser, OBJECT))
		return (INVALID_INPUT);
	parser->n_spheres++; // validate sphere

	return (NO_ERROR);
}
#else
int	parse_sphere(t_parser *parser, char *str, size_t line_num)
{
	t_object	*sphere;

	sphere = NULL;

	// initialize sphere

	if (create_new_object_node(parser) == -1)
		return (ALLOCATION_FAILURE);

	sphere = &parser->current->object;

	sphere->type = SPHERE;


	// start parsing 'str'
	skip_whitespace_but_not_newline(&str);

	// parse x,y,z coordinates of the center of the sphere (t_vec 'pos')
	if (parse_3d_vector(&str, &sphere->pos, line_num) == -1)
		return (INVALID_INPUT);
	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (INVALID_INPUT);

	skip_whitespace_but_not_newline(&str);

	// parse the sphere's diameter, and convert it to radius (double 'r')
	if (ft_strtod(&str, &sphere->r, line_num) == -1)
		return (INVALID_INPUT);
	if (sphere->r < EPSILON)  // No need to use fabs() here, since a negative value does not make sense for the diameter.
	{
		display_parsing_error("Unable to render sphere: diameter provided "
			"has to be a positive value, not too close to zero. See line:", line_num);
		return (INVALID_INPUT);
	}
	sphere->r *= 0.5; // convert diameter to radius.

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);

	// parse R,G,B colors in range [0-255]
	if (parse_color(&str, &sphere->color, NULL, line_num) == -1)
		return (INVALID_INPUT);



	// WARN: we need both elements - IF there IS a TEXTURE.
	// Accept: - no texture.
	//	- texture starting with the 'normal/ axis' vector + string (texture file). no extension.
	//	NOTE: if there is an axis vector but not the string : refuse input.
	//	NOTE: if the string is first: refuse input.
	// WARN: what if the file provided already has a file extension? shouldn't we
	// overwrite that extension, and replace it with the required suffix (_color.png || _normal.png)???

	skip_whitespace_but_not_newline(&str);
	if (!*str || *str == '\n') // no texture was provided, which is accepted! a texture is EXTRA stuff.
	{
		// check that not too many objects were provided by the user, before
		// incrementing their counter.
		if (!is_valid_n_elements(parser, OBJECT))
			return (INVALID_INPUT);
		parser->n_spheres++; // validate sphere
		return (NO_ERROR);
	}

	// TODO: parse texture string.
	// NOTE: consider a sphere without an axis but without the string!
	// TODO: write this function, that will combine both parsing parts - AXIS & STRING, since we want them both to show up.

	// WARN: if we do not handle a wrong file name,  and then call
	// mlx_load_png() ---> check that it does not segfault. Probably not, though.

	int	retval;

	retval = parse_texture_for_sphere(&str, sphere, line_num);
	if (retval)
	{
		free_all_textures_from_linked_list(parser->head);
		return (retval);
	}


	// FIXME: new draft is up until here.

	if (!is_valid_end_of_line(str, line_num))
		return (INVALID_INPUT);

	// check that not too many objects were provided by the user, before
	// incrementing their counter.
	if (!is_valid_n_elements(parser, OBJECT))
		return (INVALID_INPUT);
	parser->n_spheres++; // validate sphere

	return (NO_ERROR);
}

static void	free_all_textures_from_linked_list(t_node_obj *p_head)
{
	t_object	*p_object;

	while (p_head)
	{
		if (p_head->object.has_tex)
		{
			p_object = &p_head->object;
			free(p_object->tex_file);
			free(p_object->normal_file);
			mlx_delete_texture(p_object->texture);
			mlx_delete_texture(p_object->normal);
		}
		p_head = p_head->next;
	}
}
#endif

int	parse_plane(t_parser *parser, char *str, size_t line_num)
{
	t_object	*plane;

	plane = NULL;

	if (create_new_object_node(parser) == -1)
		return (ALLOCATION_FAILURE);

	plane = &parser->current->object;
	plane->type = PLANE;

	// start parsing 'str'
	skip_whitespace_but_not_newline(&str);

	// parse x,y,z coordinates of a point in the plane (t_vec 'pos')
	if (parse_3d_vector(&str, &plane->pos, line_num) == -1)
		return (INVALID_INPUT);

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);

	// parse 3d normalized axis vector. In range [-1,1] for each x,y,z axis (t_vec 'axis')

	if (parse_3d_vector(&str, &plane->axis, line_num) == -1)
		return (INVALID_INPUT);

	if (!is_within_range_vector(&plane->axis, line_num))
		return (INVALID_INPUT);

	if (fabs(plane->axis.x) < EPSILON && fabs(plane->axis.y) < EPSILON
		&& fabs(plane->axis.y) < EPSILON)
	{
		display_parsing_error("Provided normal vector for plane has a "
			"magnitude of zero; Unable to render object. See line:", line_num);
		return (INVALID_INPUT);
	}
	plane->axis = normalize(plane->axis);

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);

	// parse R,G,B colors in range [0-255]
	if (parse_color(&str, &plane->color, NULL, line_num) == -1)
		return (INVALID_INPUT);

	if (!is_valid_end_of_line(str, line_num))
		return (INVALID_INPUT);

	// check that not too many objects were provided by the user, before
	// incrementing their counter; otherwise overflow would occur and memory
	// would leak
	if (!is_valid_n_elements(parser, OBJECT))
		return (INVALID_INPUT);
	parser->n_planes++; // validate plane

	return (NO_ERROR);
}

int	parse_cylinder(t_parser *parser, char *str, size_t line_num)
{
	t_object	*cylinder;

	cylinder = NULL;

	if (create_new_object_node(parser) == -1)
		return (ALLOCATION_FAILURE);

	cylinder = &parser->current->object;
	cylinder->type = CYLINDER;

	// start parsing 'str'
	skip_whitespace_but_not_newline(&str);

	// parse x,y,z coordinates of the center of the cylinder (t_vec 'pos')

	if (parse_3d_vector(&str, &cylinder->pos, line_num) == -1)
		return (INVALID_INPUT);

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);

	// parse three dimensional normalized vector of the cylinder's axis (t_vec 'axis')
	// it is not an actual normal, but the direction of the central axis of the cylinder.
	if (parse_3d_vector(&str, &cylinder->axis, line_num) == -1)
		return (INVALID_INPUT);
	if (!is_within_range_vector(&cylinder->axis, line_num))
		return (INVALID_INPUT);
	if (fabs(cylinder->axis.x) < EPSILON && fabs(cylinder->axis.y) < EPSILON
		&& fabs(cylinder->axis.z) < EPSILON)
	{
		display_parsing_error("Provided vector for cylinder's axis has a "
			"magnitude of zero; Unable to render object. See line:", line_num);
		return (INVALID_INPUT);
	}
	cylinder->axis = normalize(cylinder->axis);

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);

	// parse the cylinder's diameter, and halve it, storing the RADIUS instead
	// of the given diameter, in double 'r'.

	if (ft_strtod(&str, &cylinder->r, line_num) == -1)
		return (INVALID_INPUT);
	if (cylinder->r < EPSILON)
	{
		display_parsing_error("Unable to render cylinder: diameter provided "
			"has to be a positive value, not too close to zero. See line:", line_num);
		return (INVALID_INPUT);
	}
	cylinder->r *= 0.5;

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);

	// parse the cylinder's full height, and halve it as well, storing half the
	// given height into double 'h'

	if (ft_strtod(&str, &cylinder->h, line_num) == -1)
		return (INVALID_INPUT);
	if (cylinder->h < EPSILON)
	{
		display_parsing_error("Height of cylinder is practically nonexistent; "
			"Unable to render object. See line:", line_num);
		return (INVALID_INPUT);
	}
	cylinder->h *= 0.5;

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);

	// parse the cylinder's color, into t_vec/ t_color 'color'

	if (parse_color(&str, &cylinder->color, NULL, line_num) == -1)
		return (INVALID_INPUT);

	if (!is_valid_end_of_line(str, line_num))
		return (INVALID_INPUT);

	// check that not too many objects were provided by the user, before
	// incrementing their counter; otherwise overflow would occur and memory
	// would leak
	if (!is_valid_n_elements(parser, OBJECT))
		return (INVALID_INPUT);
	parser->n_cylinders++; // validate cylinder

	return (NO_ERROR);
}

static int	create_new_object_node(t_parser *parser)
{
	t_node_obj	*new_node;

	new_node = (t_node_obj *) ft_calloc(1, sizeof (t_node_obj));
	if (!new_node)
		return (-1);
	if (!parser->head)
		parser->head = new_node;
	else
		parser->current->next = new_node;
	parser->current = new_node;
	return (0);
}
