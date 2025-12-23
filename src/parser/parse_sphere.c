/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sphere.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 22:39:34 by ykadosh           #+#    #+#             */
/*   Updated: 2025/12/21 22:03:53 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#ifndef BONUS

int	parse_sphere(t_parser *parser, char *str, size_t line_num)
{
	if (create_new_object_node(parser) == -1)
		return (ALLOCATION_FAILURE);
	parser->current->object.type = SPHERE;
	skip_whitespace_but_not_newline(&str);
	if (parse_3d_vector(&str, &parser->current->object.pos, line_num) == -1)
		return (INVALID_INPUT);
	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);
	if (set_radius_or_height(&str, parser, &parser->current->object.r, 1) == -1)
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);
	if (parse_color(&str, &parser->current->object.color, NULL, line_num) == -1)
		return (INVALID_INPUT);
	if (validate_object(str, parser) == -1)
		return (INVALID_INPUT);
	return (NO_ERROR);
}
#else

int	parse_sphere(t_parser *parser, char *str, size_t line_num)
{
	int	retval;

	if (create_new_object_node(parser) == -1)
		return (ALLOCATION_FAILURE);
	parser->current->object.type = SPHERE;
	skip_whitespace_but_not_newline(&str);
	if (parse_3d_vector(&str, &parser->current->object.pos, line_num) == -1)
		return (INVALID_INPUT);
	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);
	if (set_radius_or_height(&str, parser, &parser->current->object.r, 1) == -1)
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);
	if (parse_color(&str, &parser->current->object.color, NULL, line_num) == -1)
		return (INVALID_INPUT);
	retval = check_for_texture_and_handle(&str, parser);
	if (retval)
		return (retval);
	if (validate_object(str, parser) == -1)
		return (INVALID_INPUT);
	return (NO_ERROR);
}

int	check_for_texture_and_handle(char **str, t_parser *parser)
{
	if (isspace_but_not_newline(**str))
	{
		skip_whitespace_but_not_newline(str);
		if (**str && **str != '\n')
			return (parse_texture(str, &parser->current->object,
					parser->line_num));
	}
	else if (**str && **str != '\n')
	{
		display_parsing_error("Unknown character at tail end of numerical "
			"value, on line:", parser->line_num);
		return (INVALID_INPUT);
	}
	return (0);
}
#endif
