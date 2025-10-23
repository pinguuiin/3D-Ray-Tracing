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

static inline void	put_pos_nbr_endl_fd(uint32_t n, int fd);

/*
* 'error_code' with the value 1 signifies that there has been a misconfiguration
* in the input file, and that an error message has already been displayed on the
* standard error - a message which always begins with "Error", followed by a
* newline and by a more specific message attempting to describe the error (and
* displaying the number of line on which it occured).
* All other cases are rather fatal system errors, and their error messages are
* not preceded by the string "Error\n".
* This function can serve in providing the exit status for the program, by
* passing its call as an argument to exit(). Invalid input returns 2, while
* a system error returns 3.
*/
int	handle_parsing_error(int error_code, char *line, t_parser *parser)
{
	clean_up_parsing_memory(parser, line);
	if (error_code == 1)
		return (2);
	if (error_code == -1)
		ft_putendl_fd(2, "Dynamic memory allocation request has failed.");
	else if (error_code == -2)
		ft_putendl_fd(2, "System call failed; Unable to read input file data.");
	else if (error_code == -3)
		ft_putendl_fd(2, "Failed to process input file; buffer size is empty.");
	else if (error_code == -4)
		ft_putendl_fd("Failed to open input file.");
	ft_putendl_fd(2, "Aborting miniRT.");
	return (3);
}

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

// WARN: REMEMBER TO ADJUST THIS FUNCTION EVERY TIME free_exit() IS MODIFIED!!!
void	clean_up_parsing_memory(t_parser *parser, char *line)
{
	// WARN: # 2: you need to adjust your freeing function to use the very same
	// insertion_point pointer. At the start of the freeing function, reset
	// that pointer to head's address. And walk the list using that double pointer,
	// assigning to it the next pointer every time, until it is pointing to NULL.
	// No need for an extra variable.

	t_node_light	*current;
	t_node_light	*next;

	current = NULL;
	if (parser->head)
	{
		current = parser->head;
		while (current)
		{
			next = current->next;
			free(current);
			current = next;
		}
	}

	// free object list	// FIXME:

	// free the returned line (from get_next_line_minirt())
	free(line);


	/*
	 * WARN: I am pretty sure this function is only used BEFORE MLX initialization.
	 * so no need to make those checks. But let's double check later.
	if (info->img)
		mlx_delete_image(info->mlx, info->img);
	if (info->mlx)
		mlx_terminate(info->mlx);
	*/
}

void	display_parsing_error(const char *msg, uint32_t line_num)
{
	write(2, "Error\n", 6);
	ft_putstr_fd(msg, 2);
	write(fd, " ", 1);
	put_pos_nbr_endl_fd(line_num, 2);
}

/*
* Writes to the file descriptor 'fd' the number 'n', followed by a period and
* a newline character.
* 'n' should only be a non-negative value.
*/
static inline void	put_pos_nbr_endl_fd(uint32_t n, int fd)
{
	uint64_t	x;
	int			len;
	int			temp_len;
	char		str[11];	// max length for UINT_MAX is 10 digits.

	// only non-negative numbers shall be passed to this function,
	// so there is no need to check for overflow of the negative value

	x = 10;
	len = 1;

	// calculate number of digits ('len')
	while (x <= n)
	{
		len++;
		x *= 10;
	}
	temp_len = len;

	// store the digits conversion into the string 'str',
	// from the last digit to the first.
	while (temp_len)
	{
		str[temp_len - 1] = n % 10 + '0';
		n /= 10;
		temp_len--;
	}

	// put: the converted string -> a dot and a newline charactaer
	write(fd, str, len);
	write(fd, ".\n", 2);
}

bool	is_valid_separator(char	**str)
{
	if (**str == ',')
		(*str)++;
	else
		return (0);
	if (**str == ' ')
		(*str)++;
	return (1);
}

// Checks the character to which str is pointing at, advances it by one byte
// and returns 1 if the character is expected, or returns 0 if it is unexpected.
// The use case for this function should respect the following aspects:
// - following a call to ft_strtod(), parse_color() or parse_coordinates() // WARN: add appropriate functions if there are any new ones!
// - when miniRT expects more data in the line, after the last numerical value
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
