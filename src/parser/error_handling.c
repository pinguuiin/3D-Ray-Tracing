/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 20:35:36 by ykadosh           #+#    #+#             */
/*   Updated: 2025/10/24 20:35:58 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static inline void	put_pos_nbr_endl_fd(size_t n, int fd);

/*
* 'error_code' with the value INVALID_INPUT signifies that a misconfiguration
* in the input file was detected, in which case an error message has already
* been displayed on the standard error, by the point this function is called.
* That message always begins with "Error", followed by a newline and by a more
* specific message attempting to describe the error, and displaying the number
* of line on which it occured, when appropriate.
* All other cases are rather fatal system errors, and their error messages do
* not start by the string "Error\n", in order to differentiate them with user
* input errors.
* This function can serve in providing the exit status for the program, by
* passing its call as an argument to exit(). Invalid input returns the macro
* INPUT_ERROR, while a system error returns SYSTEM_FAILURE, and, if BONUS is
* defined, a failure to load a texture by an MLX function returns MLX_FAILURE.
*
* Note that if 'status' holds the macro value for OPEN_FAILURE, opening the
* input file has failed, meaning that no dynamic allocation has taken place, and
* therefore, calling clean_up_parser() is not necessary and even dangerous.
*
* Furthermore, in cases where 'status' holds INVALID_INPUT, CLOSE_FAILURE,
* ALLOCATION_FAILURE or LOAD_TEXTURE_FAIL, an error message has already been
* printed to the standard error, and this function does not need to handle that.
*/
#ifndef BONUS

int	handle_parsing_error(t_status status, t_parser *parser)
{
	if (status == OPEN_FAILURE)
	{
		ft_putstr_fd("Failed to open input file. Please make sure the file "
			"exists and that the correct path is provided.\n", 2);
		return (SYSTEM_FAILURE);
	}
	if (clean_up_parser(parser) == CLOSE_FAILURE)
		return (SYSTEM_FAILURE);
	if (status == INVALID_INPUT)
		return (INPUT_ERROR);
	if (status == ALLOCATION_FAILURE)
		ft_putstr_fd("Dynamic memory allocation request has failed. ", 2);
	else if (status == READ_FAILURE)
		ft_putstr_fd("System call failed; Unable to read input file data. ", 2);
	else if (status == BUFFER_SIZE_ERROR)
		ft_putstr_fd("Failed to process input file; buffer size is empty. ", 2);
	ft_putstr_fd("Aborting miniRT.\n", 2);
	return (SYSTEM_FAILURE);
}
#else

int	handle_parsing_error(t_status status, t_parser *parser)
{
	if (status == OPEN_FAILURE)
	{
		ft_putstr_fd("Failed to open input file. Please make sure the file "
			"exists and that the correct path is provided.\n", 2);
		return (SYSTEM_FAILURE);
	}
	if (clean_up_parser(parser) == CLOSE_FAILURE)
		return (SYSTEM_FAILURE);
	if (status == INVALID_INPUT)
		return (INPUT_ERROR);
	if (status == LOAD_TEXTURE_FAIL)
		return (MLX_FAILURE);
	if (status == ALLOCATION_FAILURE)
		ft_putstr_fd("Dynamic memory allocation request has failed. ", 2);
	else if (status == READ_FAILURE)
		ft_putstr_fd("System call failed; Unable to read input file data. ", 2);
	else if (status == BUFFER_SIZE_ERROR)
		ft_putstr_fd("Failed to process input file; buffer size is empty. ", 2);
	ft_putstr_fd("Aborting miniRT.\n", 2);
	return (SYSTEM_FAILURE);
}
#endif

void	display_parsing_error(char *msg, size_t line_num)
{
	write(2, "Error\n", 6);
	ft_putstr_fd(msg, 2);
	write(2, " ", 1);
	put_pos_nbr_endl_fd(line_num, 2);
}

/*
* Writes to the file descriptor 'fd' the number 'n', followed by a period and
* a newline character.
*
* WARN: Parameter 'n' should ONLY be a non-negative value, when passed to this
* function. This function does NOT check for overflow of negative values.
*
* Regarding the allocation of 21 for 'str': Since SIZE_MAX can only be up to 20
* digits long, it is safe to allocate 21 bytes for its converted string - 20
* bytes plus room for the null terminator.
*/
static inline void	put_pos_nbr_endl_fd(size_t n, int fd)
{
	size_t	x;
	int		len;
	int		temp_len;
	char	str[21];

	x = n / 10;
	len = 1;
	while (x > 0)
	{
		len++;
		x /= 10;
	}
	temp_len = len;
	while (temp_len)
	{
		temp_len--;
		str[temp_len] = n % 10 + '0';
		n /= 10;
	}
	write(fd, str, len);
	write(fd, ".\n", 2);
}
