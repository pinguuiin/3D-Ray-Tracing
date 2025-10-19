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

#include "minirt.h"

static inline double	str_degrees_to_radians(char **str, uint32_t line_num);

// NOTE: In all functions of parse_setting.c and parse_objects.c:
// The pointer 'str' is always pointing one byte past the scene's type
// identifier ('A', 'C', 'L', "sp", "pl" or "cy") AND the whitespace
// (non-newline) character that follows it! So you still need to parse
// through potential isspace_but_not_newline().
int	parse_ambient_lighting(t_ambient *amb, char *str, uint32_t line_num)
{
	// check if we already have ambient lighting: Only 1 is accepted
	if (amb->is_provided)
	{
		display_parsing_error("Too many ambient lighting sources provided; "
			"Invalid input on line number", line_num);
		return (1);
	}
	while (isspace_but_not_newline(*str))
		str++;

	if (ft_strtod(&str, &amb->ratio, line_num) == -1)
		return (1);

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (1);
	while (isspace_but_not_newline(*str))
		str++;

	if (parse_color(&str, &amb->color) == -1)
	{
		display_parsing_error("Invalid input for color values.\nPlease use "
			"three values in range 0 to 255, separated by commas, on line:",
			line_num);
		return (1);
	}

	if (!is_valid_end_of_line(str))
		return (1);
	amb->is_provided = 1; // validate the ambient lighting
	return (0);
}

// TODO:

// TODO: Use the following template for invalid camera attributes:
/*
	{
		display_parsing_error("Invalid camera attributes provided, on line"
			line_num);
		return (1);
	}
*/
int	parse_camera(t_cam *cam, char *str, uint32_t line_num)
{
	// check if we already have a camera: Only 1 is accepted
	if (cam->is_provided)
	{
		display_parsing_error("Too many cameras suggested by the input file; "
			"Invalid input at line number", line_num);
		return (1);
	}
	while (isspace_but_not_newline(*str))
		str++;

	if (parse_coordinates(&str, &cam->pos, line_num) == -1)
		return (1);

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (1);
	while (isspace_but_not_newline(*str))
		str++;

	if (parse_direction_vector(&str, &cam->direction, line_num) == -1)
		return (1);

	// if {0.0,0.0,0.0} is provided, set it to the default direction: {0.0,0.0,1.0}
	if (fabs(cam->direction.x) < EPSILON && fabs(cam->direction.y) < EPSILON
		&& fabs(cam->direction->z) < EPSILON)
		cam->direction.z = 1.0;
	cam->direction = normalize(cam->direction); // normalize
	
	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (1);
	while (isspace_but_not_newline(*str))
		str++;

	// FIXME: is it okay that I only accept integers and no floats for this?
	cam->fov = str_degrees_to_radians(&str, line_num);
	if (cam->fov < 0.0)
		return (1);

	if (!is_valid_end_of_line(str))
		return (1);
	cam->is_provided = 1;	// validate the camera
	return (0);
}

int	parse_light(t_light *light, char *str, uint32_t line_num)
{
	// check if we already have a light source: Only 1 is accepted
	if (light)
	{
		display_parsing_error("Too many light sources present in the scene; "
			"Invalid input found at line number", line_num);
		return (1);

	}
	while (isspace_but_not_newline(*str))
		str++;

}

// FIXME: if M_PI does not compile, you may need to either:
// 1. before the #include <math.h>, add: #define _USE_MATH_DEFINES
// 2. do a regular ifndef sequence with the value : PI 3.14159265358979323846
static inline double	str_degrees_to_radians(char **str, uint32_t line_num)
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
