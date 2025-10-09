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
// (non-newline) character that follows it!
int	parse_ambient_lighting(t_amb *ambient, char *str, uint32_t line_num)
{
	// check if we already have ambient lighting: Only 1 is accepted
	// FIXME: but is 'ambient' even set to NULL when declared? Or, isn't
	// an ambient struct with only zeroes a potentially valid element??
	// This idea is wrong. Find more precise solution! For all element parsing
	// functions.
	if (ambient)
	{
		display_parsing_error("Too many ambient lighting sources provided; "
			"Invalid input on line number", line_num);
		return (1);
	}
	while (isspace_but_not_newline(*str))
		str++;

	// TODO: parse one single ambient lighting ratio in range [0.0,1.0]
	// careful not to segfault here, if input is invalid and we are already on
	// the '\n' or '\0'? You still need to do error handling as 'str' might be
	// pointing at a '\n', a random undesired character, or '\0'.
	


	if (ft_strtod(&str, &amb->ratio) == -1)
	{
		display_parsing_error("Invalid input around real number, on line:", line_num);
		//  TODO: Should this rather be done within ft_strtod()?
		// That would however imply passing line_num into there...
		return (1);
	}

	// important check, since ft_strtod() only checks if the number's tail
	// is strange - but accepts null terminator or newline as valid endings.
	// We might even have a situation where the program arrives to this check
	// when the string is: "A \n", and this error should be handled.
	if (!*str || *str == '\n')
	{
		display_parsing_error("Missing data for ambient lighting provided. "
		"See line number:", line_num);
		return (1);
	}

	while (isspace_but_not_newline(*str))
		str++;

	// TODO: parse R,G,B colors in range [0,255] (normalize them between 0.0 and 1.0)
	// careful not to segfault here, if input is invalid and we are already on
	// the '\n' or '\0'?
	// str_to_normalized_rgb(&str, ???);



	while (ft_isspace(*str)) // advance the pointer until one byte after the '\n'.
		str++;
	if (*str)	// there is still data in the line, before its newline: error.
	{
		display_parsing_error("Unknown data encountered on line number",
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
