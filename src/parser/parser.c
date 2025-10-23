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

static int	parse_line(t_parser *parser, char *line, uint32_t line_num);

void	parse_scene(char *file_name)
{
	t_parser	parser;
	int			fd;
	char		*line;
	uint32_t	line_num; // always positive!
	int			err_code;

	line = NULL;
	line_num = 1;
	err_code = 0;

	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		err_code = -4;
	while (!err_code)
	{
		err_code = get_next_line_minirt(fd, &line);

		if (!err_code)
		{
			if (line)
			{
				err_code = parse_line(&parser, line, line_num);
				if (!err_code)
				{
					free(line);
					line = NULL;
					if (line_num < UINT32_MAX) // just to avoid a 'potential' segfault
						line_num++;
				}
			}
			else	// file has been fully read.
			{
				// TODO: check here (or right after the call to this function):
				// Do we have all the elements needed to render a scene, even though
				// parsing might have seemed successful? Do we have a light? Do
				// we have ambient lighting? Do we have a camera? Do we have sufficient
				// objects (what is the minimum required - should we accept a
				// scene with 0 objects?)

				// FIXME:
				// once you transfer the objects list/s into into the array of all of the
				// objects' structs, organize them in order ----> the count for each will
				// allow us to optimize.

				return ;
			}
		}
	}
	if (err_code)
		exit(handle_fatal_parsing_error(err_code, line, &parser));

}

// FIXME: consider doing the isspace_but_not_newline() checks from the specific
// parsing functions, to make this look more clean -> and to go with the general
// "atoi()" type logic....
static int	parse_line(t_parser *parser, char *line, uint32_t line_num)
{
	t_info	*info;
	int		err_code;
	char	*str;	// used to be able to free 'line'

	info = get_info();
	str = line;
	err_code = 0;

	// initialize parser struct
	ft_bzero(&parser, sizeof (t_parser));

	skip_whitespace_but_not_newline(&str);

	if (*str == '#' || *str == '\n')	// ignore comments in .rt file || line is 'empty' but valid
		return ;
	else if (*str == 'A' && isspace_but_not_newline(*(str + 1)))
		err_code = parse_ambient_lighting(&info->amb, str + 2, line_num);
	else if (*str == 'C' && isspace_but_not_newline(*(str + 1)))
		err_code = parse_camera(&info->cam, str + 2, line_num);
	else if (*str == 'L' && isspace_but_not_newline(*(str + 1)))
		err_code = parse_light(&parser, str + 2, line_num);
	else if (*str == 's' && *(str + 1) == 'p'
		&& isspace_but_not_newline(*(str + 2)))
		err_code = parse_sphere(str + 3, line_num);
	else if (*str == 'p' && *(str + 1) == 'l'
		&& isspace_but_not_newline(*(str + 2)))
		err_code = parse_plane(str + 3, line_num);
	else if (*str == 'c' && *(str + 1) == 'y'
		&& isspace_but_not_newline(*(str + 2)))
		err_code = parse_cylinder(str + 3, line_num);
	else
	{
	// The next boolean check is important, because if it returns false, we have
	// an empty whitespace line ending with the EOF, which might simply be the
	// input file's very last line. That should not be considered as a parsing
	// error, and the program should proceed.
		if (*str)
		{
			err_code = 1;
			display_parsing_error("Unexpected input provided on line number",
				line_num);
		}
	}
	return (err_code);
}
