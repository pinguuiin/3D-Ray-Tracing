/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 08:07:59 by ykadosh           #+#    #+#             */
/*   Updated: 2025/10/03 19:26:57 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
* returns true if the character is: space, newline ('\n'), form-feed ('\f'),
* horizontal tab ('\t'), vertical tab ('\v')  or carriage return ('\r')
*/
int	ft_isspace(int c)
{
	if (c == ' ' || (c >= '\t' && c <= '\r'))
		return (1);
	return (0);
}

/*
* returns true if the character is one of the following: space, form-feed
* ('\f'), horizontal tab ('\t'), vertical tab ('\v') or carriage return ('\r')
*/
int	isspace_but_not_newline(int c)
{
	if (c == ' ' || c == '\t' || (c >= '\v' && c <= '\r'))
		return (1);
	return (0);
}

void	skip_whitespace(char **str)
{
	char	*s;

	s = *str;
	while (*s == ' ' || (*s >= '\t' && *s <= '\r'))
		s++;
	*str = s;
}

void	skip_whitespace_but_not_newline(char **str)
{
	char	*s;

	s = *str;
	while (*s == ' ' || *s == '\t' || (*s >= '\v' && *s <= '\r'))
		s++;
	*str = s;
}

bool	is_valid_separator(char	**str, uint32_t line_num)
{
	if (**str == ',')
		(*str)++;
	else
	{
		display_parsing_error("Invalid input when expecting a comma to "
			"separate between elements of a triad of values.\n"
			"See line number:", line_num);
		return (0);
	}
	if (**str == ' ')
		(*str)++;
	return (1);
}

// Checks the character to which str is pointing at, advances it by one byte
// and returns 1 if the character is expected, or returns 0 if it is unexpected.
// The use case for this function should respect both of the following aspects:
// - right after a call to ft_strtod(), parse_color() or parse_3d_vector()
// - when miniRT expects yet more data after the last value that has been parsed
bool	is_valid_tail_when_expecting_more_data(char **str, uint32_t line_num)
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
bool	is_valid_end_of_line(char *s)
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
