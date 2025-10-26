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
		display_parsing_error("Invalid diameter provided for sphere object; "
			"It has to be a postive value. See line number", line_num);
		return (1);
	}
	sphere->r *= .50; // convert diameter to radius.

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (1);
	skip_whitespace_but_not_newline(&str);

	// parse R,G,B colors in range [0-255]
	if (parse_color(&str, &sphere->color, NULL, line_num) == -1)
		return (1);

	if (!is_valid_end_of_line(str))
		return (1);

	parser->n_objs++; // validate sphere.
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

	/*
	// FIXME: are we sure not to accept all zeroes ?
	// UPDATE: No, that was only valid for the camera!!!
	// From Ping:
	// "for plane and cylinder, if they don't have a proper normal vector, we return error"
	// BUT WHAT IS NOT A PROPER NORMAL VECTOR?????????????????
	// if {0.0,0.0,0.0} is provided, set it to the default direction: {0.0,0.0,1.0}
	if (fabs(cam->direction.x) < EPSILON && fabs(cam->direction.y) < EPSILON
		&& fabs(cam->direction->z) < EPSILON)
		cam->direction.z = 1.0;

	cam->direction = normalize(cam->direction); // normalize
	*/

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (1);
	skip_whitespace_but_not_newline(&str);


	// parse R,G,B colors in range [0-255]
	if (parse_color(&str, &plane->color, NULL, line_num) == -1)
		return (1);

	if (!is_valid_end_of_line(str))
		return (1);

	parser->n_objs++; // validate plane
	return (0);
}

int	parse_cylinder(t_parser *parser, char *str, uint32_t line_num)
{
	t_object	*cylinder;

	cylinder = NULL;
	if (create_new_object_node(parser) == -1)
		return (-1);
	cylinder = &parser->curr_obj->object;
	cylinder->type = CYLINDER;



	skip_whitespace_but_not_newline(&str);












	parser->n_objs++; // validate object
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
