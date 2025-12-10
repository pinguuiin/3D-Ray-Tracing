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

static int				validate_ratio(double ratio, size_t line_num);
static inline double	str_degrees_to_radians(char **str, size_t line_num);

int	parse_ambient_lighting(t_color *amb, char *str, t_parser *parser)
{
	double	ratio;

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
	if (!validate_ratio(ratio, parser->line_num))
		return (INVALID_INPUT);
	if (!is_valid_tail_when_expecting_more_data(&str, parser->line_num))
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);
	if (parse_color(&str, amb, &ratio, parser->line_num) == -1)
		return (INVALID_INPUT);
	if (!is_valid_end_of_line(str, parser->line_num))
		return (INVALID_INPUT);
	parser->n_ambs++;
	return (NO_ERROR);
}

int	parse_camera(t_cam *cam, char *str, t_parser *parser)
{
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
	if (!validate_vector(&cam->direction, parser->line_num, CAM_DIRECTION))
		return (INVALID_INPUT);
	if (!is_valid_tail_when_expecting_more_data(&str, parser->line_num))
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);
	cam->fov = str_degrees_to_radians(&str, parser->line_num);
	if (cam->fov < 0.0)
		return (INVALID_INPUT);
	if (!is_valid_end_of_line(str, parser->line_num))
		return (INVALID_INPUT);
	parser->n_cams++;
	return (NO_ERROR);
}

/*
* NOTE: While miniRT's mandatory assignment only accepts a single fixed light
* source (in addition to the ambient light), the so-called 'bonus' part can
* accept multiple lights. This is why there are two different versions for this
* function: for the base implementation, the light source is a single 't_light'
* struct, and if a light has already been parsed, a second gets rejected here.
* In the more elaborated version, lights are allocated into a linked list, just
* like the objects - before being transferred into an array, once parsing is
* complete and rendering is about to start.
*/
#ifndef BONUS
int	parse_light(t_parser *parser, char *str, t_light *light)
{
	double	ratio;

	// check if we already have a light source: Only 1 is accepted in the mandatory part.
	if (parser->n_lights) // the mandatory part only accepts one single light source
	{
		display_parsing_error("Too many light sources present in the scene; "
			"Only one fixed light is accepted. See line", parser->line_num);
		return (INVALID_INPUT);
	}

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
	if (!validate_ratio(ratio, parser->line_num))
		return (INVALID_INPUT);

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
#else
int	parse_light(t_parser *parser, char *str)
{
	double	ratio;
	t_light	*light; // shortcut, for readability

	light = NULL; // can be omitted

	if (create_new_light_node(parser) == -1)
		return (ALLOCATION_FAILURE);

	light = &parser->curr_light->light;

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
	if (!validate_ratio(ratio, parser->line_num))
		return (INVALID_INPUT);
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
		if (!is_valid_n_elements(parser, LIGHT))
			return (INVALID_INPUT);
		parser->n_lights++; // keep count of valid lights
		return (NO_ERROR);
	}
	display_parsing_error("Unexpected input found at tail end of light "
		"source's ratio value, on line", parser->line_num);
	return (INVALID_INPUT);
}
#endif

static int	validate_ratio(double ratio, size_t line_num)
{
	if (ratio < 0.0 || ratio > 1.0)
	{
		display_parsing_error("Value provided for light's brightness is out "
			"of range. Allowed range: 0.0 to 1.0. See line", line_num);
		return (0);
	}
	return (1);
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
		angle = angle * 10 + *s++ - '0';
		if (angle > 180)
		{
			display_parsing_error("Camera's field of view is beyond accepted "
				"range [0,180], on line", line_num);
			return (-1.0);
		}
	}
	*str = s;
	return (angle * M_PI / 180.0);
}
