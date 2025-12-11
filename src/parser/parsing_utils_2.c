/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:23:47 by ykadosh           #+#    #+#             */
/*   Updated: 2025/12/11 23:02:44 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
* Checks the character to which str is pointing at, advances it by one byte
* and returns 1 if the character is expected, or 0 if it is not.
* The use case for this function should respect both of the following aspects:
* - It should be deployed right after a call to ft_strtod(), parse_color() or
*   parse_3d_vector()
* - when miniRT expects yet more data after the last value that has been parsed
*/
bool	is_valid_tail_when_expecting_more_data(char **str, size_t line_num)
{
	if (isspace_but_not_newline(**str))
	{
		(*str)++;
		return (1);
	}
	else if (!**str || **str == '\n')
		display_parsing_error("Missing data for element on line:", line_num);
	else
		display_parsing_error("Unknown character at tail end of numerical "
			"value, on line:", line_num);
	return (0);
}

/*
* To be used after the last data in a given line (of the input .rt file) has
* been successfully parsed, and no more data is expected.
* Since miniRT used get_next_line() to parse the input .rt file, this function
* allows detection of any unwanted input at the end of the line - whether it
* is the first character that 's' points to at the start of the function, or
* after a series of whitespace. Should the line be valid, however, 's' ends up
* pointing at the nul-terminator (that either follows the newline character, or
* not, in case it is the last line of the file, and that line does not have a
* newline character at all).
*/
bool	is_valid_end_of_line(char *s, size_t line_num)
{
	skip_whitespace(&s);
	if (*s)
	{
		display_parsing_error("Unexpected data encountered on line number",
			line_num);
		return (0);
	}
	return (1);
}

bool	is_valid_n_elements(t_parser *parser, t_list_id id)
{
	int	n_elements;

	if (id == OBJECT)
		n_elements = parser->n_spheres + parser->n_planes + parser->n_cylinders;
	else
		n_elements = parser->n_lights;
	if (n_elements == INT_MAX)
	{
		if (id == OBJECT)
			ft_putstr_fd("Error\nToo many objects provided by input file. "
				"Only up to INT_MAX objects are accepted.\n", 2);
		else
			ft_putstr_fd("Error\nToo many light sources provided by input "
				"file. Only up to INT_MAX lights are accepted.\n", 2);
		return (0);
	}
	return (1);
}

/*
* parses the given object's diameter or height, checks whether it is valid (only
* positive values which are not too close to zero are accepted), and converts
* it to a radius (by halving the value), since the radius (or half the height)
* is more useful for miniRT's rendering calculations.
*/
int	set_radius_or_height(char **str, t_parser *parser,
				double *r_or_h, bool is_diameter)
{
	if (ft_strtod(str, r_or_h, parser->line_num) == -1)
		return (-1);
	if (*r_or_h < EPSILON)
	{
		if (parser->current->object.type == SPHERE)
			display_parsing_error("Unable to render sphere: diameter provided "
				"has to be a positive value, not too close to zero. See line:",
				parser->line_num);
		else
		{
			if (is_diameter)
				display_parsing_error("Unable to render cylinder: diameter "
					"provided has to be a positive value, not too close to "
					"zero. See line:", parser->line_num);
			else
				display_parsing_error("Height of cylinder is practically "
					"nonexistent; Unable to render object. See line:",
					parser->line_num);
		}
		return (-1);
	}
	*r_or_h *= 0.5;
	if (!is_valid_tail_when_expecting_more_data(str, parser->line_num))
		return (-1);
	return (0);
}

/*
* Unlike the many parsing functions of this ray tracer program, this function
* (and is_valid_end_of_line(), which it calls) do not take a double pointer to
* the string, because they only serve to validate (or invalidate) the end of the
* line, and no more parsing of any elements is actually done.
*/
int	validate_object(char *str, t_parser *parser)
{
	if (!is_valid_end_of_line(str, parser->line_num))
		return (-1);
	if (!is_valid_n_elements(parser, OBJECT))
		return (-1);
	if (parser->current->object.type == SPHERE)
		parser->n_spheres++;
	else if (parser->current->object.type == PLANE)
		parser->n_planes++;
	else if (parser->current->object.type == CYLINDER)
		parser->n_cylinders++;
	return (0);
}
