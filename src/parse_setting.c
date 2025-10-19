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

	// WARN: consider merging this block into one function, with the almost identical block in parse_ambient_lighting()
	// its use is always:
	// - right after a call to ft_strtod()
	// - if there should be more data in the line, after the converted float
	// - if there is a strange character right after the float's very last digit
	if (!*str || *str == '\n') // if true, .rt file is lacking RGB data for amb!
	{
		display_parsing_error("Missing data for ambient lighting element. "
		"See line number:", line_num);
		return (1);
	}
	else if (!isspace_but_not_newline(*str)) // if true, the floating point ratio has a strange tail. ft_strtod() does not check for this.
	{
		display_parsing_error("Unknown input when expecting floating point "
			"number, on line:", line_num);
		return (1);
	}

	while (isspace_but_not_newline(*str))
		str++;

	if (parse_color(&str, &amb->color) == -1)
	{
		display_parsing_error("Invalid input for color values.\nPlease use "
			"three values in range 0 to 255, separated by commas, on line:",
			line_num);
		return (1);
	}

	// advance the pointer until one byte after the '\n', since miniRT employs
	// get_next_line() - or, if there is still some unwanted input in the
	// string, the pointer will point to it
	while (ft_isspace(*str))
		str++;
	if (*str)	// there is still data in the line, before its newline - or the color values have a strange tail: error.
	{
		display_parsing_error("Unexpected data encountered on line number",
			line_num);
		return (1);
	}
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

	// WARN: consider merging this block into one function, with the almost identical block in parse_ambient_lighting()
	// its use is always:
	// - right after a call to ft_strtod()
	// - if there should be more data in the line, after the converted float
	// - if there is a strange character right after the float's very last digit
	if (!*str || *str == '\n') // if true, .rt file is lacking attributes for the camera!
	{
		display_parsing_error("Missing data for camera, on line:", line_num);
		return (1);
	}
	else if (!isspace_but_not_newline(*str)) // if true, the last coordinate value has a strange tail. ft_strtod() does not check for this.
	{
		display_parsing_error("Unknown input when expecting floating point "
			"number, on line:", line_num);
		return (1);
	}

	// parse more whitespace but not newline
	while (isspace_but_not_newline(*str))
		str++;

	// TODO: Parse "t_vec direction" normalized orientation vector, in range [-1,1] for each x,y,z axis:
	// WARN: How about a negative "z" direction? The camera is facing backwards?
	// update: Yes, it can face backwards! The camera might be in the middle of
	// the room, and there may or may not be objects behind it, which you would
	// see if the camera faces backwards.
	// WARN: is there something here regarding a sum equal to 1?
	// update: these values should be normalized into a scale of  [-1,1] BEFORE
	// the rendering happens, so it would be great to normalize them already here.
	// BUT - for the moment it is already being done after the parsing,
	// so discuss this in the team and decide together where to keep it!
	if (parse_direction_vector(&str, &cam->direction, line_num) == -1)
		return (1);

	// if {0.0,0.0,0.0} is provided, set it to the default direction: {0.0,0.0,1.0}
	if (fabs(cam->direction.x) < EPSILON && fabs(cam->direction.y) < EPSILON
		&& fabs(cam->direction->z) < EPSILON)
		cam->direction.z = 1.0;
	cam->direction = normalize(cam->direction); // normalize
	

	// TODO: validate input by checking the character to which str is pointing to right now!
	// WARN: consider merging this block into one function, with the almost identical block in parse_ambient_lighting()
	// its use is always:
	// - right after a call to ft_strtod()
	// - if there should be more data in the line, after the converted float
	// - if there is a strange character right after the float's very last digit
	if (!*str || *str == '\n') // if true, .rt file is lacking attributes for the camera!
	{
		display_parsing_error("Missing data for camera, on line:", line_num);
		return (1);
	}
	else if (!isspace_but_not_newline(*str)) // if true, the last orientation value has a strange tail. ft_strtod() does not check for this.
	{
		display_parsing_error("Unknown input when expecting floating point "
			"number, on line:", line_num);
		return (1);
	}

	// parse more whitespace but not newline
	while (isspace_but_not_newline(*str))
		str++;

	// TODO: parse ("double fov"): provided in degrees in range [0,180],
	// but should be converted to rad: provided angle * π / 180.
	// accept 0.0 for now, but remember testing it. Subject specifies "in range [0,180]"
	// so better accept it, but check that nothing breaks later on when you run the program.


	// advance the pointer until one byte after the '\n', since miniRT employs
	// get_next_line() - or, if there is still some unwanted input in the
	// string, the pointer will point to it
	while (ft_isspace(*str))
		str++;
	if (*str)	// there is still data in the line, before its newline: error.
	{
		display_parsing_error("Unexpected data encountered on line number",
			line_num);
		return (1);
	}
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
