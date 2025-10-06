/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 21:47:12 by ykadosh           #+#    #+#             */
/*   Updated: 2025/10/06 19:56:10 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	parse_line(t_info *info, char *line, uint32_t line_num);

void	parse_scene(t_info *info, char *file_name)
{
	int			fd;
	char		*line;
	uint_32t	line_num; // always positive!
	int			gnl_flag;

	line = NULL;
	line_num = 1;
	gnl_flag = 0;

	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		free_exit(info, "Failed to open input file. Aborting program");
	while (!gnl_flag)
	{
		gnl_flag = get_next_line_minirt(fd, &line);

		if (!gnl_flag)
		{
			if (line)
			{
				parse_line(info, line, line_num);
				free(line);
				line = NULL;
				if (line_num < UINT_MAX) // just to avoid potential segfault...
					line_num++;
			}
			else	// file has been fully read.
			{
				// TODO: check here (or right after the call to this function):
				// Do we have all the elements needed to render a scene, even though
				// parsing might have seemed successful? Do we have a light? Do
				// we have ambient lighting? Do we have a camera? Do we have sufficient
				// objects (what is the minimum required - should we accept a
				// scene with 0 objects?)
				return ;
			}
		}

		/*
		// alternate version of the above:
		if (!gnl_flag && line)
		{
			parse_line(info, line, line_num);
			free(line);
			line = NULL;
			line_num++;
		}
		else if (!gnl_flag && !line) // if this check is not done ---> infinite loop once we finished reading!
			return ;
		*/
	}
	if (gnl_flag)
		handle_gnl_error_and_exit(info, gnl_flag);

	// FIXME:
	// once you transfer the objects list/s into into the array of all of the
	// objects' structs, organize them in order ----> the count for each will
	// allow us to optimize.
}

// Alternate version:
// if an invalid input is found: exit status is 2
static void	parse_line(t_info *info, char *line, uint32_t line_num)
{
	int		is_invalid;
	char	*str;	// used to be able to free line

	str = line;
	is_invalid = 0;
	while (isspace_but_not_newline(*str))
		str++;
	if (*str == '#' || *str == '\n')	// ignore comments in .rt file || line is 'empty' but valid
		return ;
	else if (*str == 'A' && isspace_but_not_newline(*(str + 1)))
		is_invalid = parse_ambient_lighting(&info->amb, str + 2, line_num);
	else if (*str == 'C' && isspace_but_not_newline(*(str + 1)))
		is_invalid = parse_camera(&info->cam, str + 2, line_num);
	else if (*str == 'L' && isspace_but_not_newline(*(str + 1)))
		is_invalid = parser_light(&info->light, str + 2, line_num);
	else if (*str == 's' && *(str + 1) == 'p'
		&& isspace_but_not_newline(*(str + 2)))
		is_invalid = parse_sphere(str + 3, line_num);
	else if (*str == 'p' && *(str + 1) == 'l'
		&& isspace_but_not_newline(*(str + 2)))
		is_invalid = parse_plane(str + 3, line_num);
	else if (*str == 'c' && *(str + 1) == 'y'
		&& isspace_but_not_newline(*(str + 2)))
		is_invalid = parse_cylinder(str + 3, line_num);
	else
	{
	// The next boolean check is important, because if it returns false, we have
	// an empty whitespace line ending with the EOF, which might simply be the
	// input file's very last line. That should not be considered as a parsing
	// error, and the program should proceed.
		if (*str)
		{
			is_invalid = 1;
			display_parsing_error("Unexpected input provided at the start of "
				"line number:", line_num);
		}
	}

	// TODO: write the error message in each and every parsing function,
	// return true, and then everything gets freed here, with exit status 2.
	if (is_invalid)
	{
		free(line);
		clean_up_parsing_memory(info);
		exit (2);
	}

}
