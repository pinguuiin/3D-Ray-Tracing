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
int	parse_ambient_lighting(t_color *amb, char *str, uint32_t line_num)
{
	static uint32_t	n_ambients;
	double			ratio;

	// check if we already have ambient lighting: Only 1 is accepted
	if (n_ambients)
	{
		display_parsing_error("Too many ambient lighting sources provided; "
			"Invalid input on line number", line_num);
		return (1);
	}
	skip_whitespace_but_not_newline(&str);

	ratio = 0.0;
	if (ft_strtod(&str, &ratio, line_num) == -1)
		return (1);
	if (ratio < 0.0 || ratio > 1.0)
	{
		display_parsing_error("Value provided for light source's brightness "
			"is out of range. Allowed range: 0.0 to 1.0. See line", line_num);
		return (1);
	}


	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (1);

	skip_whitespace_but_not_newline(&str);

	if (parse_color(&str, amb) == -1)
	{
		display_parsing_error("Invalid input for color values.\nPlease use "
			"three values in range 0 to 255, separated by commas, on line:",
			line_num);
		return (1);
	}
	// FIXME: apply ratio to r, g & b channels.
	// write a separate function for it! AND, perhaps pass a pointer to ratio to parse_color(), and
	// just do it from there -> pass to it NULL when you don't need to do that multiplication!


	if (!is_valid_end_of_line(str))
		return (1);
	n_ambients++;	// validate the ambient lighting
	return (0);
}

int	parse_camera(t_cam *cam, char *str, uint32_t line_num)
{
	static int	n_cameras;

	// check if we already have a camera: Only 1 is accepted
	if (n_cameras)
	{
		display_parsing_error("Too many cameras suggested by the input file; "
			"Invalid input at line number", line_num);
		return (1);
	}

	skip_whitespace_but_not_newline(&str);

	if (parse_coordinates(&str, &cam->pos, line_num) == -1)
		return (1);

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (1);

	skip_whitespace_but_not_newline(&str);

	if (parse_direction_vector(&str, &cam->direction, line_num) == -1)
		return (1);

	// if {0.0,0.0,0.0} is provided, set it to the default direction: {0.0,0.0,1.0}
	if (fabs(cam->direction.x) < EPSILON && fabs(cam->direction.y) < EPSILON
		&& fabs(cam->direction->z) < EPSILON)
		cam->direction.z = 1.0;
	cam->direction = normalize(cam->direction); // normalize
	
	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (1);

	skip_whitespace_but_not_newline(&str);

	cam->fov = str_degrees_to_radians(&str, line_num);
	if (cam->fov < 0.0)
		return (1);

	if (!is_valid_end_of_line(str))
		return (1);
	n_cameras++;	// validate the camera
	return (0);
}

// TODO: When working on the bonus part:
// Set 'light' as an array, like the object array.
// This will be useful for the bonus part, since it could allow several key
// light sources. It would be easier to transform to it if lights (not ambient
// lights) are stored as an array
// NOTE: note the other important distinction beetween mandatory and bonus
// parts: the color data is unused in the mandatory, but perhaps could still
// be accepted -> while the bonus (or is it only some bonus parts) require/s it!
int	parse_light(t_light *light, char *str, uint32_t line_num)
{
	static int	n_lights;
	double		ratio;

	// check if we already have a light source: Only 1 is accepted
	if (n_lights) // the mandatory part only accepts one single light source
	{
		display_parsing_error("Too many light sources present in the scene; "
			"Only one fixed light is accepted. See line", line_num);
		return (1);
	}
	skip_whitespace_but_not_newline(&str);

	// parse coordinates of the light point
	if (parse_coordinates(&str, &light->pos, line_num) == -1)
		return (1);

	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (1);
	skip_whitespace_but_not_newline(&str);

	// prase the light brightness ratio in range [0.0,1.0]
	ratio = 0.0;
	if (ft_strtod(&str, &ratio, line_num) == -1)
		return (1);
	if (ratio < 0.0 || ratio > 1.0)
	{
		display_parsing_error("Value provided for light source's brightness "
			"is out of range. Allowed range: 0.0 to 1.0. See line", line_num);
		return (1);
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
			if (parse_color(&str, &light->color) == -1)
			{
				display_parsing_error("Invalid input for color values.\nPlease "
					"use three values in range 0 to 255, separated by commas, "
					"on line:", line_num);
				return (1);
			}
			// FIXME: apply ratio to r, g & b channels.
			// write a separate function for it!
			if (!is_valid_end_of_line(str))
				return (1);
		}
		else
		{
			// set all color channels to white as a default color
			// when none are provided (but adapting the ratio already here) 
			// FIXME: use a separate function for this!
			light->color.r = 1.0 * ratio;
			light->color.g = 1.0 * ratio;
			light->color.b = 1.0 * ratio;
		}
		n_lights++;
		return (0);
	}
	display_parsing_error("Unexpected input found at tail end of light "
		"source's ratio value, on line", line_num);
	return (1);
}

/*
* Note for users: If M_PI does not compile, you may need to modify the header:
* 1. add before '#include <math.h>': #define _USE_MATH_DEFINES, or
* 2. add an 'ifndef' sequence with: PI 3.14159265358979323846
*/
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
