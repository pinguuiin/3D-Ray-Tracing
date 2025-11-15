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

#include "parser.h"

static int	parse_line(t_parser *parser, char *line);
static int	check_validity_of_scene(t_parser *parser);
#ifndef BONUS
static int	transfer_obj_list_to_array(t_parser *parser, t_info *info);
#else
static int	transfer_lists_to_arrays(t_parser *parser, t_info *info);
static void	copy_light(t_parser *parser, t_info *info);
#endif
static void	copy_obj(t_type id, t_parser *parser, int *i, int n_obj);

#ifndef BONUS
void	parse_scene(t_info *info, char *filename)
{
	t_parser	parser;
	char		*line;
	t_status	status;

	line = NULL;
	status = NO_ERROR;

	// initialize parser struct
	ft_bzero(&parser, sizeof (t_parser));
	parser.line_num = 1;

	parser.fd = open(filename, O_RDONLY);
	if (parser.fd == -1)
		status = OPEN_FAILURE;
	while (status == NO_ERROR)
	{
		status = get_next_line_revised(parser.fd, &line);

		if (status == NO_ERROR)
		{
			if (line)
			{
				status = parse_line(&parser, line);
				if (status == NO_ERROR)
				{
					free(line);
					line = NULL;
					if (parser.line_num < SIZE_MAX) // just to avoid overflow
						parser.line_num++;
				}
			}
			else	// file has been fully read.
			{
				status = check_validity_of_scene(&parser);

				// transfer the lights and all objects linked lists into their respective arrays;
				if (status == NO_ERROR)
					status = transfer_obj_list_to_array(&parser, info);

				// destroy the lists and return (unless malloc() failure has occured,
				// in which case status is ALLOCATION_FAILURE, which will be handled after the loop.
				if (status == NO_ERROR)
				{
					// destroy both linked lists, and close the file descriptor
					if (clean_up_parser(&parser, NULL) == CLOSE_FAILURE) // if true, close() failed, but memory has been freed. Time to exit.
						exit (SYSTEM_FAILURE);
					return ;
				}
			}
		}
	}
	if (status != NO_ERROR)
		exit(handle_parsing_error(status, line, &parser));
}
#else
void	parse_scene(t_info *info, char *filename)
{
	t_parser	parser;
	char		*line;
	t_status	status;

	line = NULL;
	status = NO_ERROR;

	// initialize parser struct
	ft_bzero(&parser, sizeof (t_parser));
	parser.line_num = 1;

	parser.fd = open(filename, O_RDONLY);
	if (parser.fd == -1)
		status = OPEN_FAILURE;
	while (status == NO_ERROR)
	{
		status = get_next_line_revised(parser.fd, &line);

		if (status == NO_ERROR)
		{
			if (line)
			{
				status = parse_line(&parser, line);
				if (status == NO_ERROR)
				{
					free(line);
					line = NULL;
					if (parser.line_num < SIZE_MAX) // just to avoid overflow
						parser.line_num++;
				}
			}
			else	// file has been fully read.
			{
				status = check_validity_of_scene(&parser);

				// transfer the lights and all objects linked lists into their respective arrays;
				if (status == NO_ERROR)
					status = transfer_lists_to_arrays(&parser, info);

				// destroy the lists and return (unless malloc() failure has occured,
				// in which case status is ALLOCATION_FAILURE, which will be handled after the loop.
				if (status == NO_ERROR)
				{
					// destroy both linked lists, and close the file descriptor
					if (clean_up_parser(&parser, NULL) == CLOSE_FAILURE) // if true, close() failed, but memory has been freed. Time to exit.
						exit (SYSTEM_FAILURE);
					return ;
				}
			}
		}
	}
	if (status != NO_ERROR)
		exit(handle_parsing_error(status, line, &parser));
}
#endif

// FIXME: consider doing the isspace_but_not_newline() checks from the specific
// parsing functions, to make this look more clean -> and to go with the general
// "atoi()" type logic....

#ifndef BONUS
static int	parse_line(t_parser *parser, char *line)
{
	t_info	*info;
	char	*str;	// used to be able to free 'line'

	info = get_info();
	str = line;

	skip_whitespace_but_not_newline(&str);

	if (*str == '#' || *str == '\n')	// ignore comments in .rt file || line is 'empty' but valid
		return (0);
	else if (*str == 'A' && isspace_but_not_newline(*(str + 1)))
		return (parse_ambient_lighting(&info->amb, str + 2, parser));
	else if (*str == 'C' && isspace_but_not_newline(*(str + 1)))
		return (parse_camera(&info->cam, str + 2, parser));
	else if (*str == 'L' && isspace_but_not_newline(*(str + 1)))
		return (parse_light(parser, str + 2, &info->light));
	else if (*str == 's' && *(str + 1) == 'p'
		&& isspace_but_not_newline(*(str + 2)))
		return (parse_sphere(parser, str + 3, parser->line_num));
	else if (*str == 'p' && *(str + 1) == 'l'
		&& isspace_but_not_newline(*(str + 2)))
		return (parse_plane(parser, str + 3, parser->line_num));
	else if (*str == 'c' && *(str + 1) == 'y'
		&& isspace_but_not_newline(*(str + 2)))
		return (parse_cylinder(parser, str + 3, parser->line_num));
	// The next boolean check is important, because if it returns false, we have
	// an empty whitespace line ending with the EOF, which might simply be the
	// input file's very last line. That should not be considered as a parsing
	// error, and the program should proceed.
	if (*str)
	{
		display_parsing_error("Unexpected input provided on line number",
			parser->line_num);
		return (INVALID_INPUT);
	}
	return (NO_ERROR);
}
#else
static int	parse_line(t_parser *parser, char *line)
{
	t_info	*info;
	char	*str;	// used to be able to free 'line'

	info = get_info();
	str = line;

	skip_whitespace_but_not_newline(&str);

	if (*str == '#' || *str == '\n')	// ignore comments in .rt file || line is 'empty' but valid
		return (0);
	else if (*str == 'A' && isspace_but_not_newline(*(str + 1)))
		return (parse_ambient_lighting(&info->amb, str + 2, parser));
	else if (*str == 'C' && isspace_but_not_newline(*(str + 1)))
		return (parse_camera(&info->cam, str + 2, parser));
	else if (*str == 'L' && isspace_but_not_newline(*(str + 1)))
		return (parse_light(parser, str + 2));
	else if (*str == 's' && *(str + 1) == 'p'
		&& isspace_but_not_newline(*(str + 2)))
		return (parse_sphere(parser, str + 3, parser->line_num));
	else if (*str == 'p' && *(str + 1) == 'l'
		&& isspace_but_not_newline(*(str + 2)))
		return (parse_plane(parser, str + 3, parser->line_num));
	else if (*str == 'c' && *(str + 1) == 'y'
		&& isspace_but_not_newline(*(str + 2)))
		return (parse_cylinder(parser, str + 3, parser->line_num));
	// The next boolean check is important, because if it returns false, we have
	// an empty whitespace line ending with the EOF, which might simply be the
	// input file's very last line. That should not be considered as a parsing
	// error, and the program should proceed.
	if (*str)
	{
		display_parsing_error("Unexpected input provided on line number",
			parser->line_num);
		return (INVALID_INPUT);
	}
	return (NO_ERROR);
}
#endif

#ifndef BONUS
static int	transfer_obj_list_to_array(t_parser *parser, t_info *info)
{
	int	i;

	// update n_obj in 'info'
	info->n_obj = parser->n_spheres + parser->n_planes + parser->n_cylinders;

	// allocate array of objects. make ft_calloc() failure check!
	info->obj = (t_object *) ft_calloc(info->n_obj, sizeof (t_object));
	if (!info->obj)
		return (ALLOCATION_FAILURE);

	i = 0;

	// copy all spheres' data to the START of the array.
	copy_obj(SPHERE, parser, &i, parser->n_spheres);

	// copy all planes' data to the MIDDLE of the array.
	// do NOT reset i, it keeps incrementing through the array.
	copy_obj(PLANE, parser, &i, parser->n_planes);

	// copy all cylinders' data to the END of the array.
	copy_obj(CYLINDER, parser, &i, parser->n_cylinders);

	return (NO_ERROR);
}
#else
static int	transfer_lists_to_arrays(t_parser *parser, t_info *info)
{
	int	i;

	// update n_light in 'info'
	info->n_light = parser->n_lights;

	// allocate array of lights in 'info'

	info->light = (t_light *) ft_calloc(parser->n_lights, sizeof (t_light));
	if (!info->light)
		return (ALLOCATION_FAILURE);


	// copy 'light' linked list data into the new 'light' array
	copy_light(parser, info);

	// update n_obj in 'info'
	info->n_obj = parser->n_spheres + parser->n_planes + parser->n_cylinders;

	// allocate array of objects. make ft_calloc() failure check!
	info->obj = (t_object *) ft_calloc(info->n_obj, sizeof (t_object));
	if (!info->obj)
		return (ALLOCATION_FAILURE);


	i = 0;

	// copy all spheres' data to the START of the array.
	copy_obj(SPHERE, parser, &i, parser->n_spheres);

	// copy all planes' data to the MIDDLE of the array.
	// do NOT reset i, it keeps incrementing through the array.
	copy_obj(PLANE, parser, &i, parser->n_planes);

	// copy all cylinders' data to the END of the array.
	copy_obj(CYLINDER, parser, &i, parser->n_cylinders);

	return (NO_ERROR);
}
#endif

#ifndef BONUS
#else
static void	copy_light(t_parser *parser, t_info *info)
{
	t_node_light	*current;
	int				i;

	current = parser->head_light;
	i = 0;
	while (current)
	{
		info->light[i] = current->light;
		i++;
		current = current->next;
	}
}
#endif

static void	copy_obj(t_type id, t_parser *parser, int *i, int n_obj)
{
	t_node_obj	*current;
	t_info		*info;
	int			j;

	info = get_info();
	j = *i;
	current = parser->head;

	while (current && n_obj)
	{
		if (current->object.type == id)
		{
			// copy whole object struct data
			info->obj[j] = current->object;
			j++;
			n_obj--;
		}
		current = current->next;
	}
	*i = j;
}

static int	check_validity_of_scene(t_parser *parser)
{
	int	n_objects;

	n_objects = parser->n_spheres + parser->n_planes + parser->n_cylinders;
	if (!parser->n_lights || !parser->n_cams || !n_objects)
	{
		if (!parser->n_lights && !parser->n_cams && !n_objects)
			ft_putstr_fd("Error\nAll crucial elements are missing from scene - "
				"no camera, no light source and no object found!\n", 2);
		else if (!parser->n_lights && !parser->n_cams)
			ft_putstr_fd("Error\n"
				"Scene description file has no light source nor camera.\n", 2);
		else if (!parser->n_cams && !n_objects)
			ft_putstr_fd("Error\nInput file has no camera nor object.\n", 2);
		else if (!parser->n_lights && !n_objects)
			ft_putstr_fd("Error\nNo object nor light source detected.\n", 2);
		else if (!parser->n_lights)
			ft_putstr_fd("Error\nInput .rt file has no light source.\n", 2);
		else if (!parser->n_cams)
			ft_putstr_fd("Error\nIncomplete file: no camera found.\n", 2);
		else if (!n_objects)
			ft_putstr_fd("Error\nProvided scene has no objects. At least one "
				"object should be proposed for the scene to be rendered.\n", 2);
		return (INVALID_INPUT);
	}
	return (NO_ERROR);
}
