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

#include "minirt.h"

static inline void	put_pos_nbr_endl_fd(uint32_t n, int fd);

/*
* 'error_code' with the value 1 signifies that there has been a misconfiguration
* in the input file, and that an error message has already been displayed on the
* standard error by other functions. That message always begins with "Error",
* followed by a newline and by a more specific message attempting to describe
* the error (and displaying the number of line on which it occured).
* All other cases are rather fatal system errors, and their error messages are
* not preceded by the string "Error\n".
* This function can serve in providing the exit status for the program, by
* passing its call as an argument to exit(). Invalid input returns 2, while
* a system error returns 3.
*/
// FIXME: check all calls to this function, now that we close(fd) from here!
// FIXME: Also, move int fd into the parser struct.
int	handle_parsing_error(int error_code, char *line, t_parser *parser)
{
	// FIXME: not sure about close() failure at all!!!!
	clean_up_parsing_memory(parser, line);

	if (close(parser->fd) == -1)
	{
		ft_putendl_fd("Failed to close input file.", 2);
		error_code = -5;
	}


	if (error_code == 1)
		return (2);
	if (error_code == -1)
		ft_putendl_fd("Dynamic memory allocation request has failed.", 2);
	else if (error_code == -2)
		ft_putendl_fd("System call failed; Unable to read input file data.", 2);
	else if (error_code == -3)
		ft_putendl_fd("Failed to process input file; buffer size is empty.", 2);
	else if (error_code == -4)
		ft_putendl_fd("Failed to open input file.", 2);
	ft_putendl_fd("Aborting miniRT.", 2);
	return (3);
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

// WARN: REMEMBER TO ADJUST THIS FUNCTION EVERY TIME free_exit() IS MODIFIED!!!
void	clean_up_parsing_memory(t_parser *parser, char *line)
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

	// free the returned line obtained by get_next_line_minirt()
	if (line)
		free(line);

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
}
