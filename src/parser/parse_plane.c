/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_plane.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 22:37:26 by ykadosh           #+#    #+#             */
/*   Updated: 2025/12/11 22:39:19 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#ifndef BONUS

int	parse_plane(t_parser *parser, char *str, size_t line_num)
{
	if (create_new_object_node(parser) == -1)
		return (ALLOCATION_FAILURE);
	parser->current->object.type = PLANE;
	skip_whitespace_but_not_newline(&str);
	if (parse_3d_vector(&str, &parser->current->object.pos, line_num) == -1)
		return (INVALID_INPUT);
	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);
	if (parse_and_normalize_vector(&str, &parser->current->object.axis,
			line_num, PLANE_NORMAL) == -1)
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);
	if (parse_color(&str, &parser->current->object.color, NULL, line_num) == -1)
		return (INVALID_INPUT);
	if (validate_object(str, parser) == -1)
		return (INVALID_INPUT);
	return (NO_ERROR);
}
#else

// TODO: alternate version for parse_plane, accepting textures.
int	parse_plane(t_parser *parser, char *str, size_t line_num)
{
	int	retval;

	if (create_new_object_node(parser) == -1)
		return (ALLOCATION_FAILURE);
	parser->current->object.type = PLANE;
	skip_whitespace_but_not_newline(&str);
	if (parse_3d_vector(&str, &parser->current->object.pos, line_num) == -1)
		return (INVALID_INPUT);
	if (!is_valid_tail_when_expecting_more_data(&str, line_num))
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);
	if (parse_and_normalize_vector(&str, &parser->current->object.axis,
			line_num, PLANE_NORMAL) == -1)
		return (INVALID_INPUT);
	skip_whitespace_but_not_newline(&str);
	if (parse_color(&str, &parser->current->object.color, NULL, line_num) == -1)
		return (INVALID_INPUT);

	// TODO: add parsing for texture here.
	// TODO: add check for the first character - with the option of having a texture,
	// or not having one - in which case the character could be: \n, \0, OR a whitespace followed
	// by as many whitespace as you want (except \n) up until either a \n or a \0, and that's it!
	//
	// WARN: insufficient version, ameliorated in the block below it:
	/*
	skip_whitespace_but_not_newline(&str);
	retval = handle_texture(&str, parser);
	if (retval)
		return (retval);
	if (validate_object(str, parser) == -1)
		return (INVALID_INPUT);
	return (NO_ERROR);
	*/

	
	retval = check_for_texture_and_handle(&str, parser);
	if (retval)
		return (retval);
	if (validate_object(str, parser) == -1)
		return (INVALID_INPUT);
	return (NO_ERROR);

}
#endif
