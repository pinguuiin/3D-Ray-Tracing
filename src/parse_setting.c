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
// The pointer 'str' is always pointing one byte past the scene's element
// identifier ('A', 'C', 'L', "sp", "pl" or "cy") AND the whitespace
// (non-newline) character that follows it!
void	parse_ambient_lighting(t_amb *amb, char *str, uint32_t line_num)
{
	// TODO: first check if we already have ambient lighting: Only 1 is accepted
	if (amb)
	{
		display_parsing_error("?????", line_num);
		return (1);

	}
	while (isspace_but_not_newline(*str))
		str++;


}

void	parse_camera(t_cam *cam, char *str, uint32_t line_num)
{
	// TODO: first check if we already have a camera: Only 1 is accepted
	if (cam)
	{
		display_parsing_error("?????", line_num);
		return (1);

	}

	// TODO: check that we have at least one character of whitespace (but not newline!!)
	while (isspace_but_not_newline(*str))
		str++;

}

void	parse_light(t_light *light, char *str, uint32_t line_num)
{
	// TODO: first check if we already have a light source: Only 1 is accepted
	if (light)
	{
		display_parsing_error("?????", line_num);
		return (1);

	}

	// TODO: check that we have at least one character of whitespace (but not newline!!)

	while (isspace_but_not_newline(*str))
		str++;

}
