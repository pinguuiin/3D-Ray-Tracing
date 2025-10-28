/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 07:32:46 by ykadosh           #+#    #+#             */
/*   Updated: 2025/10/03 19:27:00 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	create_new_object_node(t_parser *parser);
static bool	is_valid_n_objects(t_parser *parser, uint32_t line_num);

// NOTE: In all functions of parse_setting.c and parse_objects.c:
// The pointer 'str' is always pointing one byte past the scene's type
// identifier ('A', 'C', 'L', "sp", "pl" or "cy") AND the whitespace
// (non-newline) character that follows it!
int	parse_sphere(t_parser *parser, char *str, uint32_t line_num)
{
	t_object	*sphere;

	sphere = NULL;

	// initialize sphere
	if (create_new_object_node(parser) == -1)
		return (-1);
	sphere = &parser->curr_obj->object;
	sphere->type = SPHERE;


	// start parsing 'str'
	skip_whitespace_but_not_newline(&str);

	// parse x,y,z coordinates of the center of the sphere (t_vec 'pos')
	if (parse_3d_vector(&str, &sphere->pos, line_num) == -1)
		return (1);
	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (1);

	skip_whitespace_but_not_newline(&str);

	// parse the sphere's diameter, and convert it to radius (double 'r')
	if (ft_strtod(&str, &sphere->r, line_num) == -1)
		return (1);
	if (sphere->r < EPSILON)  // No need to use fabs() here, since a negative value does not make sense for the diameter.
	{
		display_parsing_error("Unable to render sphere: diameter provided "
			"has to be a positive value, for it to be a valid sphere.\n"
			"See line:", line_num);
		return (1);
	}
	sphere->r *= 0.5; // convert diameter to radius.

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (1);
	skip_whitespace_but_not_newline(&str);

	// parse R,G,B colors in range [0-255]
	if (parse_color(&str, &sphere->color, NULL, line_num) == -1)
		return (1);

	if (!is_valid_end_of_line(str))
		return (1);

	parser->n_spheres++; // validate sphere

	// check that not too many objects were provided by the user.
	if (!is_valid_n_objects(parser, line_num))
		return (1);

	return (0);
}

int	parse_plane(t_parser *parser, char *str, uint32_t line_num)
{
	t_object	*plane;

	plane = NULL;

	// initialize plane
	if (create_new_object_node(parser) == -1)
		return (-1);
	plane = &parser->curr_obj->object;
	plane->type = PLANE;

	// start parsing 'str'
	skip_whitespace_but_not_newline(&str);

	// parse x,y,z coordinates of a point in the plane (t_vec 'pos')
	if (parse_3d_vector(&str, &plane->pos, line_num) == -1)
		return (1);

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (1);
	skip_whitespace_but_not_newline(&str);

	// parse 3d normalized normal vector. In range [-1,1] for each x,y,z axis (t_vec 'normal')

	if (parse_3d_vector(&str, &plane->normal, line_num) == -1)
		return (1);

	if (fabs(plane->normal.x) < EPSILON && fabs(plane->normal.y) < EPSILON
		&& fabs(plane->normal.y < EPSILON))
	{
		display_parsing_error("Provided normal vector for plane has a "
			"magnitude of zero; Unable to render object. See line:", line_num);
		return (1);
	}
	plane->normal = normalize(plane->normal);

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (1);
	skip_whitespace_but_not_newline(&str);

	// parse R,G,B colors in range [0-255]
	if (parse_color(&str, &plane->color, NULL, line_num) == -1)
		return (1);

	if (!is_valid_end_of_line(str))
		return (1);

	parser->n_planes++; // validate plane

	// check that not too many objects were provided by the user.
	if (!is_valid_n_objects(parser, line_num))
		return (1);

	return (0);
}

int	parse_cylinder(t_parser *parser, char *str, uint32_t line_num)
{
	t_object	*cylinder;

	cylinder = NULL;

	// initialize cylinder
	if (create_new_object_node(parser) == -1)
		return (-1);
	cylinder = &parser->curr_obj->object;
	cylinder->type = CYLINDER;

	// start parsing 'str'
	skip_whitespace_but_not_newline(&str);

	// parse x,y,z coordinates of the center of the cylinder (t_vec 'pos')

	if (parse_3d_vector(&str, &cylinder->pos, line_num) == -1)
		return (1);

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (1);
	skip_whitespace_but_not_newline(&str);

	// parse three dimensional normalized vector of the cylinder's axis (t_vec 'normal')
	// it is not an actual normal, but the direction of the central axis of the cylinder.
	if (parse_3d_vector(&str, &cylinder->normal, line_num) == -1)
		return (1);
	if (fabs(cylinder->normal.x) < EPSILON && fabs(cylinder->normal.y) < EPSILON
		&& fabs(cylinder->normal.z) < EPSILON)
	{
		display_parsing_error("Provided vector for cylinder's axis has a "
			"magnitude of zero; Unable to render object. See line:", line_num);
		return (1);
	}
	cylinder->normal = normalize(cylinder->normal);

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (1);
	skip_whitespace_but_not_newline(&str);

	// parse the cylinder's diameter, and halve it, storing the RADIUS instead
	// of the given diameter, in double 'r'.

	if (ft_strtod(&str, &cylinder->r, line_num) == -1)
		return (1);
	if (cylinder->r < EPSILON)
	{
		display_parsing_error("Unable to render cylinder: diameter provided "
			"has to be a positive value, for it to be a valid cylinder.\n"
			"See line:", line_num);
		return (1);
	}
	cylinder->r *= 0.5;

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (1);
	skip_whitespace_but_not_newline(&str);

	// parse the cylinder's full height, and halve it as well, storing half the
	// given height into double 'h'

	if (ft_strtod(&str, &cylinder->h, line_num) == -1)
		return (1);
	if (cylinder->h < EPSILON)
	{
		display_parsing_error("Unable to render cylinder: height provided "
			"has to be a positive value, for it to be a valid cylinder.\n"
			"See line:", line_num);
		return (1);
	}
	cylinder->h *= 0.5;

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (1);
	skip_whitespace_but_not_newline(&str);

	// parse the cylinder's color, into t_vec/ t_color 'color'

	if (parse_color(&str, &cylinder->color, NULL, line_num) == -1)
		return (1);

	if (!is_valid_end_of_line(str))
		return (1);

	parser->n_cylinders++; // validate cylinder

	// check that not too many objects were provided by the user.
	if (!is_valid_n_objects(parser, line_num))
		return (1);

	return (0);
}

static int	create_new_object_node(t_parser *parser)
{
	if (!parser->head_obj)
	{
		parser->head_obj = (t_node_obj *) ft_calloc(1, sizeof (t_node_obj));
		if (!parser->head_obj)
			return (-1);
		parser->curr_obj = parser->head_obj;
	}
	else
	{
		parser->curr_obj->next = (t_node_obj *) ft_calloc(1,
			sizeof (t_node_obj));
		if (!parser->curr_obj->next)
			return (-1);
		parser->curr_obj = parser->curr_obj->next;
	}
	return (0);
}

static bool	is_valid_n_objects(t_parser *parser, uint32_t line_num)
{
	uint8_t	n_objects;

	n_objects = parser->n_spheres + parser->n_planes + parser->cylinders;
	if (n_objects <= 30)
		return (1);
	display_parsing_error("miniRT only accepts up to 30 objects. 31st object "
		"is on line number", line_num);
	return (0);
}
