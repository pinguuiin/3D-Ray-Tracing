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
	if (create_new_object_node(parser) == -1)
		return (-1);
	sphere = &parser->curr_obj->object;
	sphere->type = SPHERE;


	skip_whitespace_but_not_newline(&str);

	// parse x,y,z coordinates of the center of the sphere (t_vec 'pos')
	if (parse_coordinates(&str, &sphere->pos, line_num) == -1)
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
	if (parse_color(&str, ) == -1) // FIXME: lacking display_error_message() for the moment!
	{
		display_parsing_error("Invalid input for color values.\nPlease use "
			"three values in range 0 to 255, separated by commas, on line:",
		return (1);
	}

	if (!is_valid_end_of_line(str))
		return (1);

	parser->n_objs++;
	return (0);
}

int	parse_plane(t_parser *parser, char *str, uint32_t line_num)
{
	t_object	*plane;

	plane = NULL;
	if (create_new_object_node(parser) == -1)
		return (-1);
	plane = &parser->curr_obj->object;
	plane->type = PLANE;




	skip_whitespace_but_not_newline(&str);






	parser->n_objs++;
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












	parser->n_objs++;
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
