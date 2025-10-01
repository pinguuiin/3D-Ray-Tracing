/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 21:47:12 by ykadosh           #+#    #+#             */
/*   Updated: 2025/09/21 21:47:53 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	handle_gnl_error_and_exit(t_info *info, int gnl_flag);
static int	parse_line(t_info *info, char *line);
static int	isspace_but_not_newline(int c);

void	parse_scene(t_info *info, char *file_name)
{
	int		fd;
	char	*line;
	int		line_num;
	int		gnl_flag;

	line = NULL;
	line_num = 1;
	gnl_flag = 0;

	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		free_exit(info, "Failed to open input file. Aborting program");
	while (!gnl_flag)
	{
		gnl_flag = get_next_line_minirt(fd, &line);
		if (!gnl_flag && line)
		{
			parse_line(info, line);
			free(line);
			line = NULL;
			line_num++;
		}
	}
	if (gnl_flag)
		handle_gnl_error_and_exit(info, gnl_flag);


	// TODO: consider allowing comments in the .rt scene text file:
	// All commented lines could, for example, start with a '#' sign - and
	// those lines would be ignored. Otherwise, the .rt file is so complicated
	// to make sense of, but comments there would be of great help! :-)

	// FIXME:
	// once you transfer the objects list/s into into the array of all of the
	// objects' structs, organize them in order ----> the count for each will
	// allow us to optimize.
}


static void	handle_gnl_error_and_exit(t_info *info, int gnl_flag)
{
	if (gnl_flag == -1)
		free_exit(info, "Dynamic memory allocation request has failed.");
	if (gnl_flag == -2)
		free_exit(info, "System call failed; Unable to read input file data");
	if (gnl_flag == -3)
		free_exit(info, "Failed to process input file; buffer size is empty");
}

// NOTE: free the line from here when an invalid input is found!
static int	parse_line(t_info *info, char *line)
{
	while (isspace_but_not_newline(*line))
		line++;
	if (*line == 'A')
		parse_ambient_lighting(&info->amb, line);
	else if (*line == 'C')
		parse_camera(&info->cam, line);
	else if (*line == 'L')
		parser_light(&info->light, line);






}

static int	isspace_but_not_newline(int c)
{
	if (c == ' ' || c == '\t' || (c >= '\v' && c <= '\r'))
		return (1);
	return (0);
}
