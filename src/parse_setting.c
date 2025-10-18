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

	if (ft_strtod(&str, &amb->ratio) == -1)
		return (1);

	// important check, since ft_strtod() only checks if the number's tail
	// is strange - but accepts null terminator or newline as valid endings,
	// which, in the context of parse_ambient_lighting() is invalid; We still
	// need the RGB data of the ambient lighting.
	if (!*str || *str == '\n')
	{
		display_parsing_error("Missing data for ambient lighting element. "
		"See line number:", line_num);
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
	if (*str)	// there is still data in the line, before its newline: error.
	{
		display_parsing_error("Unexpected data encountered on line number",
			line_num);
		return (1);
	}
	return (0);
}

int	parse_camera(t_cam *cam, char *str, uint32_t line_num)
{
	// check if we already have a camera: Only 1 is accepted
	if (cam)
	{
		display_parsing_error("Too many cameras suggested by the input file; "
			"Invalid input at line number", line_num);
		return (1);

	}
	while (isspace_but_not_newline(*str))
		str++;

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
