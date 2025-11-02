/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_setting.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 18:15:18 by ykadosh           #+#    #+#             */
/*   Updated: 2025/10/06 19:56:11 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static inline double	str_degrees_to_radians(char **str, size_t line_num);

// NOTE: In all functions of parse_setting.c and parse_objects.c:
// The pointer 'str' is always pointing one byte past the scene's type
// identifier ('A', 'C', 'L', "sp", "pl" or "cy") AND the whitespace
// (non-newline) character that follows it! So you still need to parse
// through potential isspace_but_not_newline().
int	parse_ambient_lighting(t_color *amb, char *str, t_parser *parser)
{
	double	ratio;

	// check if we already have ambient lighting: Only 1 is accepted
	if (parser->n_ambs)
	{
		display_parsing_error("Too many ambient lighting sources provided; "
			"Invalid input on line number", parser->line_num);
		return (INVALID_INPUT);
	}
	skip_whitespace_but_not_newline(&str);

	ratio = 0.0;
	if (ft_strtod(&str, &ratio, parser->line_num) == -1)
		return (INVALID_INPUT);
	if (ratio < 0.0 || ratio > 1.0)
	{
		display_parsing_error("Value provided for light source's brightness "
			"is out of range. Allowed range: 0.0 to 1.0. See line", parser->line_num);
		return (INVALID_INPUT);
	}


	if (!is_valid_tail_when_expecting_more_data(&str, parser->line_num))
		return (INVALID_INPUT);

	skip_whitespace_but_not_newline(&str);

	if (parse_color(&str, amb, &ratio, parser->line_num) == -1)
		return (INVALID_INPUT);

	if (!is_valid_end_of_line(str, parser->line_num))
		return (INVALID_INPUT);
	parser->n_ambs++;	// validate the ambient lighting
	return (NO_ERROR);
}

int	parse_camera(t_cam *cam, char *str, t_parser *parser)
{
	// check if we already have a camera: Only 1 is accepted
	if (parser->n_cams)
	{
		display_parsing_error("Too many cameras suggested by the input file; "
			"Invalid input at line number", parser->line_num);
		return (INVALID_INPUT);
	}

	skip_whitespace_but_not_newline(&str);

	if (parse_3d_vector(&str, &cam->pos, parser->line_num) == -1)
		return (INVALID_INPUT);

	if (!is_valid_tail_when_expecting_more_data(&str, parser->line_num))
		return (INVALID_INPUT);

	skip_whitespace_but_not_newline(&str);

	if (parse_3d_vector(&str, &cam->direction, parser->line_num) == -1)
		return (INVALID_INPUT);

	if (!is_within_range_vector(&cam->direction, parser->line_num))
		return (INVALID_INPUT);

	// if {0.0,0.0,0.0} is provided, set it to the default direction: {0.0,0.0,1.0}
	if (fabs(cam->direction.x) < EPSILON && fabs(cam->direction.y) < EPSILON
		&& fabs(cam->direction.z) < EPSILON)
		cam->direction.z = 1.0;
	cam->direction = normalize(cam->direction); // normalize
	
	if (!is_valid_tail_when_expecting_more_data(&str, parser->line_num))
		return (INVALID_INPUT);

	skip_whitespace_but_not_newline(&str);

	cam->fov = str_degrees_to_radians(&str, parser->line_num);
	if (cam->fov < 0.0)
		return (INVALID_INPUT);

	if (!is_valid_end_of_line(str, parser->line_num))
		return (INVALID_INPUT);
	parser->n_cams++;	// validate the camera
	return (NO_ERROR);
}

// TODO: When working on the bonus part:
// Set 'light' as an array, like the object array.
// This will be useful for the bonus part, since it could allow several key
// light sources. It would be easier to transform to it if lights (not ambient
// lights) are stored as an array
// NOTE: note the other important distinction beetween mandatory and bonus
// parts: the color data is unused in the mandatory, but perhaps could still
// be accepted -> while the bonus (or is it only some bonus parts) require/s it!
int	parse_light(t_parser *parser, char *str)
{
	double	ratio;
	t_light	*light; // shortcut, for readability

	light = NULL; // can be omitted

	/*
	 * This block is only for the mandatory part.
	// check if we already have a light source: Only 1 is accepted
	if (parser->n_lights) // the mandatory part only accepts one single light source
	{
		display_parsing_error("Too many light sources present in the scene; "
			"Only one fixed light is accepted. See line", parser->line_num);
		return (INVALID_INPUT);
	}
	*/

	// allocate new light node, check for malloc() failure
	if (create_new_node(&parser->head, &parser->current, LIGHT, sizeof (t_node_light)) == -1)
		return (ALLOCATION_FAILURE);

	/*
	 * WARN: this is the alternate version, which I most probably am going to end
	 * up using instead of the above !!!
	if (create_new_light_node(parser) == -1)
		return (ALLOCATION_FAILURE);
	*/

	light = &parser->current->light;

	// parse the string into the allocated 'light'
	skip_whitespace_but_not_newline(&str);

	// parse coordinates of the light point
	if (parse_3d_vector(&str, &light->pos, parser->line_num) == -1)
		return (INVALID_INPUT);

	if (!is_valid_tail_when_expecting_more_data(&str, parser->line_num))
		return (INVALID_INPUT);

	skip_whitespace_but_not_newline(&str);

	// prase the light brightness ratio in range [0.0,1.0]
	ratio = 0.0;
	if (ft_strtod(&str, &ratio, parser->line_num) == -1)
		return (INVALID_INPUT);
	if (ratio < 0.0 || ratio > 1.0)
	{
		display_parsing_error("Value provided for light source's brightness "
			"is out of range. Allowed range: 0.0 to 1.0. See line", parser->line_num);
		return (INVALID_INPUT);
	}

	// WARN:
	// the next data is unused in the mandatory part.
	// This means that we should accept cases where there is no more data
	// at this point! But we should not accept weird data.
	// If there is valid RGB data, we could parse it into the struct, it does
	// not matter.

	if (!*str || ft_isspace(*str))
	{
		skip_whitespace(&str);
		if (*str)
		{
			if (parse_color(&str, &light->color, &ratio, parser->line_num) == -1)
				return (INVALID_INPUT);
			if (!is_valid_end_of_line(str, parser->line_num))
				return (INVALID_INPUT);
		}
		else	// no color provided by input for 'light', so set it to white
			apply_ratio_to_color(&light->color, ratio, 0);
		parser->n_lights++; // keep count of valid lights
		return (NO_ERROR);
	}
	display_parsing_error("Unexpected input found at tail end of light "
		"source's ratio value, on line", parser->line_num);
	return (INVALID_INPUT);
}

static inline double	str_degrees_to_radians(char **str, size_t line_num)
{
	uint32_t	angle;
	char		*s;

	angle = 0;
	s = *str;
	if (*s == '+')
		s++;
	if (!ft_isdigit(*s))
	{
		display_parsing_error("Unknown input when expecting an angle value "
			"between 0 and 180, on line number", line_num);
		return (-1.0);
	}
	while (ft_isdigit(*s))
	{
		angle = angle * 10 + *s - '0';
		if (angle > 180)
		{
			display_parsing_error("Unknown input when expecting an angle value "
				"between 0 and 180, on line number", line_num);
			return (-1.0);
		}
		s++;
	}
	*str = s;
	return (angle * M_PI / 180.0);
}

/*
static int	create_new_light_node(t_parser *parser)
{
	t_node_light	*new_node;

	new_node = (t_node_light *) ft_calloc(1, sizeof (t_light_node));
	if (!new_node)
		return (-1);
	if (!parser->head)
		parser->head = new_node;
	else
		parser->current->next = new_node;
	parser->current = new_node;
	return (0);
}
*/
