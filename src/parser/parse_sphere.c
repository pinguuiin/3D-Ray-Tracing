/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sphere.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 22:39:34 by ykadosh           #+#    #+#             */
/*   Updated: 2025/12/11 23:02:11 by ykadosh          ###   ########.fr       */
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

	// TODO: add check for the first character - with the option of having a texture,
	// or not having one - in which case the character could be: \n, \0, OR a whitespace followed
	// by as many whitespace as you want (except \n) up until either a \n or a \0, and that's it!

	/*
	* NOTE: version number 1
	if (!*str || *str == '\n')
		// validate object and return
	else if (*str == '\n')
		// validate object and return
	else if (ft_isspace(*str)) // it's okay to check for '\n' because we already checked above.
	{
		skip_whitespace_but_not_newline(&str);
		if (*str && (*str != '\n'))
		{
			// retval = handle_texture(&str, parser);
			// if (retval)
				// return (retval);
		}
	}
	else
	{
		display_parsing_error("Unknown character at tail end of numerical "
			"value, on line:", line_num);
		return (INVALID_INPUT);
	}

	if (validate_object(str, parser) == -1)
		return (INVALID_INPUT);
	return (NO_ERROR);
	*/




	/*
	// TODO: alternate version
	// version number 2
	if (ft_isspace_but_not_newline(*str))
	{
		skip_whitespace_but_not_newline(&str);
		if (*str && (*str != '\n'))
		{
			retval = handle_texture(&str, parser);
			if (retval)
				return (retval);
		}
	}
	else if (*str && *str != '\n')
	{
		display_parsing_error("Unknown character at tail end of numerical "
			"value, on line:", line_num);
		return (INVALID_INPUT);
	}

	if (validate_object(str, parser) == -1)
		return (INVALID_INPUT);
	return (NO_ERROR);
	*/


	// TODO: alternate version
	// version number 3: identical to number 2 but refactored
	retval = check_for_texture_and_handle(&str, parser);
	if (retval)
		return (retval);
	if (validate_object(str, parser) == -1)
		return (INVALID_INPUT);
	return (NO_ERROR);


	// skip_whitespace_but_not_newline(&str);
	// retval = handle_texture(&str, parser);
	// if (retval)
	// 	return (retval);
	// if (validate_object(str, parser) == -1)
	// 	return (INVALID_INPUT);
	// return (NO_ERROR);
}

static void	free_all_textures_from_linked_list(t_node_obj *p_head)
{
	t_object	*p_object;

	while (p_head)
	{
		if (p_head->object.tex_file)
		{
			p_object = &p_head->object;
			free(p_object->tex_file);
			free(p_object->normal_file);
			mlx_delete_texture(p_object->texture);
			if (p_object->normal)
				mlx_delete_texture(p_object->normal);
		}
		p_head = p_head->next;
	}
}

int	handle_texture(char **str, t_parser *parser)
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

int	check_for_texture_and_handle(char **str, t_parser *parser)
{
	int	retval;

	// retval = 0;
	if (isspace_but_not_newline(**str))
	{
		skip_whitespace_but_not_newline(str);
		if (**str && **str != '\n')
		{
			retval = handle_texture(str, parser);
			return (retval); // in the caller, "if (retval) should be checked, and if true, 'retva' should be returned
			// if (retval)
			// 	return (retval);
		}
	}
	else if (**str && **str != '\n')
	{
		display_parsing_error("Unknown character at tail end of numerical "
			"value, on line:", parser->line_num);
		return (INVALID_INPUT);
	}
	return (0);
}

// FIXME: delete this block
	// if (validate_object(str, parser) == -1)
	// 	return (INVALID_INPUT);
	// return (NO_ERROR);

#endif
