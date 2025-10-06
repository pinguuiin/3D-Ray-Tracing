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

static inline void	put_space_positive_nbr_endl_fd(uint32_t n, int fd);

void	handle_gnl_error_and_exit(t_info *info, int gnl_flag)
{
	if (gnl_flag == -1)
		free_exit(info, "Dynamic memory allocation request has failed.");
	if (gnl_flag == -2)
		free_exit(info, "System call failed; Unable to read input file data");
	if (gnl_flag == -3)
		free_exit(info, "Failed to process input file; buffer size is empty");
}

int	isspace_but_not_newline(int c)
{
	if (c == ' ' || c == '\t' || (c >= '\v' && c <= '\r'))
		return (1);
	return (0);
}

// WARN: REMEMBER TO ADJUST THIS FUNCTION EVERY TIME free_exit() IS MODIFIED!!!
void	clean_up_parsing_memory(t_info *info)
{
	if (info->img)
		mlx_delete_image(info->mlx, info->img);
	if (info->mlx)
		mlx_terminate(info->mlx);
}

// WARN: checkt that this is good!
void	display_parsing_error(const char *msg, uint32_t line_num)
{
	ft_putendl_fd("Error", 2);
	ft_putstr_fd(msg, 2);
	// FIXME: replace with put_pos_nbr_endl_fd
	put_space_positive_nbr_endl_fd(line_num, 2);
	ft_putnbr_fd(line_num, 2); // check how to handle putnbr error!!
	write(2, "\n", 1);
}

/*
* Writes to the file descriptor 'fd' the number 'n', surrounded by:
* - one single space as prefix, and
* - one newline character as suffix.
* That number should only be a non-negative value!
*/
// FIXME: write this function!
static inline void	put_space_positive_nbr_endl_fd(uint32_t n, int fd)
{
	uint64_t	x;
	int			len;
	int			temp_len;

	// only non-negative numbers shall be passed to this function,
	// so there is no need to check for overflow of the negative value

	// NOTE: this is just a draft, not yet sure about this right now.
	x = 10;
	len = 1;
	while (x <= n)
	{
		len++;
		x =* 10;
	}

	temp_len = len;
	while (temp_len)
	{

		temp_len--;
	}

 
	write(fd, " .....\n", len + 1); // TODO: replace ..... with number in string format.
	// TODO: does write allow to combine the prefix ' ' and the suffix '\n' around
	// the number string, in the same call?? probably not..

}
