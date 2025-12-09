/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:23:47 by ykadosh           #+#    #+#             */
/*   Updated: 2025/12/09 17:28:52 by ykadosh          ###   ########.fr       */
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

bool	is_within_range_vector(t_vec *vec, size_t line_num)
{
	if (vec->x < -1.0 || vec->x > +1.0)
	{
		display_parsing_error("x axis of vector has to be within range "
			"[-1,+1] for miniRT to accept this file.\n"
			"This invalid input was detected on line number:", line_num);
		return (0);
	}
	if (vec->y < -1.0 || vec->y > +1.0)
	{
		display_parsing_error("y axis of vector has to be within range "
			"[-1,+1] for miniRT to accept this file.\n"
			"This invalid input was detected on line number:", line_num);
		return (0);
	}
	if (vec->z < -1.0 || vec->z > +1.0)
	{
		display_parsing_error("z axis of vector has to be within range "
			"[-1,+1] for miniRT to accept this file.\n"
			"This invalid input was detected on line number:", line_num);
		return (0);
	}
	return (1);
}
