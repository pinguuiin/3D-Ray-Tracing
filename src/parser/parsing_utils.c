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

#include "parser.h"

/*
* returns true if the character is: space, newline ('\n'), form-feed ('\f'),
* horizontal tab ('\t'), vertical tab ('\v')  or carriage return ('\r')
*/
bool	ft_isspace(int c)
{
	if (c == ' ' || (c >= '\t' && c <= '\r'))
		return (1);
	return (0);
}

/*
* returns true if the character is one of the following: space, form-feed
* ('\f'), horizontal tab ('\t'), vertical tab ('\v') or carriage return ('\r')
*/
bool	isspace_but_not_newline(int c)
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

bool	is_valid_separator(char	**str, size_t line_num)
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

// WARN: this is the crazily complicated looking function that merges create_
// new_light_node() and create_new_obj_node(), using void pointers.
// The separate functions are ready in parse_setting.c and parse_objects.c
// Please delete this if it does not work, and delete also the enum t_list_id
int	create_new_node(void *head, void *current, t_list_id id, size_t size)
{
	void	*result;

	result = ft_calloc(1, size);
	if (!result)
		return (-1);

	if (!head)
	{
		if (id == LIGHT)
		{
			*(t_node_light **)head = result;
			*(t_node_light **)current = result;
		}
		else
		{
			*(t_node_obj **)head = result;
			*(t_node_obj **)current = result;
		}
	}
	else
	{
		if (id == LIGHT)
		{
			(*(t_node_light **)current)->next = result;
			*(t_node_light **)current = (*(t_node_light **)current)->next;
		}
		else
		{
			(*(t_node_obj **)current)->next = result;
			*(t_node_obj **)current = (*(t_node_obj **)current)->next;
		}
	}
	// NOTE: remember assigning the local *light or *sphere/*plane/*cylinder to the newly allocated node, at the caller, just after this call :-)
	return (0);
}

/*
// NOTE: alternate version with local double pointers
// FIXME: delete this when not used anymore!
static int	create_new_node(void **head, void **current, t_list_id list_id)
{
	t_node_light 	**light;
	t_node_obj		**obj;

	if (list_id == LIGHT)
	{
		*light = (t_node_obj *) ft_calloc(1, sizeof(t_node_light));
		if (!*light)
			return (-1);
		
	}
	else
	{
		*obj = (t_node_obj *) ft_calloc(1, sizeof(t_node_obj));
		if (!*obj)
			return (-1);
	}




	if (list_id == LIGHT)
		*current = (t_node_light *) *current;
	else
		*current = (t_node_obj *) *current;

	if (!*head)
	{


	}


}
*/

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
