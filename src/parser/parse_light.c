/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 20:38:58 by ykadosh           #+#    #+#             */
/*   Updated: 2025/12/21 23:46:28 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	parse_brightness(char **str, t_parser *parser);
static int	handle_color_parsing(char **str, t_parser *parser, t_color *color);

int	parse_ambient_lighting(t_color *amb, char *str, t_parser *parser)
{
	if (parser->n_ambs)
	{
		display_parsing_error("Too many ambient lighting sources provided; "
			"Invalid input on line number", parser->line_num);
		return (INVALID_INPUT);
	}
	skip_whitespace_but_not_newline(&str);
	if (parse_brightness(&str, parser) == -1)
		return (INVALID_INPUT);
	if (!is_valid_tail_when_expecting_more_data(&str, parser->line_num))
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);
	if (parse_color(&str, amb, &parser->ratio, parser->line_num) == -1)
		return (INVALID_INPUT);
	if (!is_valid_end_of_line(str, parser->line_num))
		return (INVALID_INPUT);
	parser->n_ambs++;
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
	if (parser->n_lights)
	{
		display_parsing_error("Too many light sources present in the scene; "
			"Only one fixed light is accepted. See line", parser->line_num);
		return (INVALID_INPUT);
	}
	skip_whitespace_but_not_newline(&str);
	if (parse_3d_vector(&str, &light->pos, parser->line_num) == -1)
		return (INVALID_INPUT);
	if (!is_valid_tail_when_expecting_more_data(&str, parser->line_num))
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);
	if (parse_brightness(&str, parser) == -1)
		return (INVALID_INPUT);
	if (!*str || ft_isspace(*str))
	{
		if (handle_color_parsing(&str, parser, &light->color) == -1)
			return (INVALID_INPUT);
		parser->n_lights++;
		return (NO_ERROR);
	}
	display_parsing_error("Unexpected input found at tail end of light "
		"source's ratio value, on line", parser->line_num);
	return (INVALID_INPUT);
}
#else

int	parse_light(t_parser *parser, char *str)
{
	t_light	*light;

	if (create_new_light_node(parser) == -1)
		return (ALLOCATION_FAILURE);
	light = &parser->curr_light->light;
	skip_whitespace_but_not_newline(&str);
	if (parse_3d_vector(&str, &light->pos, parser->line_num) == -1)
		return (INVALID_INPUT);
	if (!is_valid_tail_when_expecting_more_data(&str, parser->line_num))
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);
	if (parse_brightness(&str, parser) == -1)
		return (INVALID_INPUT);
	if (!*str || ft_isspace(*str))
	{
		if (handle_color_parsing(&str, parser, &light->color) == -1)
			return (INVALID_INPUT);
		if (!is_valid_n_elements(parser, LIGHT))
			return (INVALID_INPUT);
		parser->n_lights++;
		return (NO_ERROR);
	}
	display_parsing_error("Unexpected input found at tail end of light "
		"source's ratio value, on line", parser->line_num);
	return (INVALID_INPUT);
}
#endif

static int	handle_color_parsing(char **str, t_parser *parser, t_color *color)
{
	skip_whitespace(str);
	if (**str)
	{
		if (parse_color(str, color, &parser->ratio, parser->line_num) == -1)
			return (-1);
		if (!is_valid_end_of_line(*str, parser->line_num))
			return (-1);
	}
	else
		apply_ratio_to_color(color, parser->ratio, 0);
	return (0);
}

static int	parse_brightness(char **str, t_parser *parser)
{
	parser->ratio = 0.0;
	if (ft_strtod(str, &parser->ratio, parser->line_num) == -1)
		return (-1);
	if (parser->ratio < 0.0 || parser->ratio > 1.0)
	{
		display_parsing_error("Value provided for light's brightness is out "
			"of range. Allowed range: 0.0 to 1.0. See line", parser->line_num);
		return (-1);
	}
	return (0);
}
