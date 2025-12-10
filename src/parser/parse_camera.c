/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_camera.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 20:39:17 by ykadosh           #+#    #+#             */
/*   Updated: 2025/12/10 20:51:20 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static double	str_degrees_to_radians(char **str, size_t line_num);

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
	if (parse_and_normalize_vector(&str, &cam->direction, parser->line_num,
			CAM_DIRECTION) == -1)
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

static double	str_degrees_to_radians(char **str, size_t line_num)
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
