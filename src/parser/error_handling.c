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

static inline void	put_pos_nbr_endl_fd(uint32_t n, int fd);

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
* INVALID_INPUT, while a system error returns SYSTEM_FAILURE.
*/
int	handle_parsing_error(t_status status, char *line, t_parser *parser)
{
	if (status == OPEN_FAILURE)
	{
		// status -4 means that open() failed. No need to call close(),
		// and nothing has been allocated yet, so no need to clean_up_parser().
		ft_putstr_fd("Failed to open input file. Aborting miniRT.\n", 2);
		return (SYSTEM_FAILURE);
	}

	// if true: close() has failed, but all the memory has been already freed, it is safe to exit
	if (clean_up_parser(parser, line) == CLOSE_FAILURE)
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

void	display_parsing_error(char *msg, uint32_t line_num)
{
	write(2, "Error\n", 6);
	ft_putstr_fd(msg, 2);
	write(2, " ", 1);
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
	char		str[11];	// max length for UINT32_MAX is 10 digits.

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

// WARN: REMEMBER TO ADJUST THIS FUNCTION EVERY TIME free_exit() IS MODIFIED!!!
/*
* This function frees the dynamically allocated memory used by parsing, i.e:
* - linked list for 'lights'
* - linked list for all 'objects'
* - 'line', returned by get_next_line_revised()
* It also closes the file descriptor associated with the input .rt file.
*
* Return Values:
*	0:	Dynamically allocated memory was properly freed and file descriptor
*		was properly closed.
*	CLOSE_FAILURE:	Allocated memory was successfully freed, but close() has
*		failed. It is safe to end the program.
*/
int	clean_up_parser(t_parser *parser, char *line)
{
	t_node_light	*current;
	t_node_light	*next;
	t_node_obj		*curr_obj;
	t_node_obj		*next_obj;

	// free linked list of lights
	current = parser->head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}

	// free linked list of objects
	curr_obj = parser->head_obj;
	while (curr_obj)
	{
		next_obj = curr_obj->next;
		free(curr_obj);
		curr_obj = next_obj;
	}

	// free the returned line obtained by get_next_line_revised()
	if (line)
		free(line);

	if (close(parser->fd) == -1)
	{
		ft_putstr_fd("Failed to close input file. Aborting miniRT.\n", 2);
		return (CLOSE_FAILURE);
	}

	/*
	 * WARN: I am pretty sure this function is only used BEFORE MLX initialization.
	 * so no need to make those checks. But let's double check later.
	 * NOTE: Also, I am pretty sure I'd like to use this function after I am done
	 * parsing a valid file, and so I for sure don't want to include this next block.
	if (info->img)
		mlx_delete_image(info->mlx, info->img);
	if (info->mlx)
		mlx_terminate(info->mlx);
	*/

	return (0);
}
