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

	skip_whitespace_but_not_newline(&str);

	if (*str && *str != '\n')
	{
		int	retval;

		retval = parse_texture_for_sphere(&str, sphere, line_num);
		if (retval)
		{
			free_all_textures_from_linked_list(parser->head);
			return (retval);
		}
	}

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
		if (p_head->object.tex_file)
		{
			p_object = &p_head->object;
			free(p_object->tex_file);
			free(p_object->normal_file);
			mlx_delete_texture(p_object->texture);
			if (p_object->normal)
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

	if (parse_and_normalize_vector(&str, &plane->axis, line_num,
		PLANE_NORMAL) == -1)
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

	if (parse_and_normalize_vector(&str, &cylinder->axis, line_num,
		CYLINDER_AXIS) == -1)
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

int	validate_vector(t_vec *vector, size_t line_num, t_vector_id id)
{
	if (id == SPHERE_AXIS)
		vector->z = 0.0;
	if (fabs(vector->x) < EPSILON && fabs(vector->y) < EPSILON
		&& fabs(vector->z) < EPSILON)
	{
		if (id == CAM_DIRECTION)
			display_parsing_error("Vector normalization failed: cannot accept "
				"a zero-length camera direction. Error on line", line_num);
		else if (id == PLANE_NORMAL)
			display_parsing_error("Vector normalization failed: cannot "
				"normalize a zero-length plane normal. See line:", line_num);
		else if (id == CYLINDER_AXIS)
			display_parsing_error("Vector normalization failed: cannot "
				"normalize a zero-length cylinder axis. See line:", line_num);
		else if (id == SPHERE_AXIS)
			display_parsing_error("Vector normalization failed: cannot accept "
				"a zero-length sphere axis.\n"
				"Please note that the z-component is ignored and parsed as 0.0 "
				"when the vector belongs to a sphere.\nSee line:", line_num);
		return (0);
	}
	*vector = normalize(*vector);
	return (1);
}

int	parse_and_normalize_vector(char **str, t_vec *vector, size_t line_num,
		t_vector_id id)
{
	if (parse_3d_vector(str, vector, line_num) == -1)
		return (-1);
	if (!is_within_range_vector(vector, line_num))
		return (-1);
	if (!validate_vector(vector, line_num, id))
		return (-1);
	if (!is_valid_tail_when_expecting_more_data(str, line_num))
		return (-1);
	return (0);
}
