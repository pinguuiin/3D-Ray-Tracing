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
