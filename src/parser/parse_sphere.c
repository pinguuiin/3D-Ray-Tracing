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

/*
 * FIXME: delete this function when ready

static void	free_all_textures_from_linked_list(t_node_obj *p_head)
{
	t_object	*p_object;

	while (p_head)
	{
		p_object = &p_head->object;
		if (p_object->tex_file)
			free(p_object->tex_file);
		if (p_object->normal_file)
			free(p_object->normal_file);
		if (p_object->texture)
			mlx_delete_texture(p_object->texture);
		if (p_object->normal)
			mlx_delete_texture(p_object->normal);
		p_head = p_head->next;
	}
}
*/

/*
static int	handle_texture(char **str, t_parser *parser)
{
	int	retval;

	retval = 0;
	if (**str && **str != '\n')
	{
		retval = parse_texture(str, &parser->current->object, parser->line_num);
		if (retval)
			free_all_textures_from_linked_list(parser->head);
	}
	return (retval);
}
*/

int	check_for_texture_and_handle(char **str, t_parser *parser)
{
	// FIXME: delete this when ready.
	// int	retval;
	//
	if (isspace_but_not_newline(**str))
	{
		skip_whitespace_but_not_newline(str);
		if (**str && **str != '\n')
			return (parse_texture(str, &parser->current->object, parser->line_num));


			/*
			 * FIXME: delete this block when ready.

			retval = handle_texture(str, parser);
			return (retval);
			*/
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
