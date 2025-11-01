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

static int	parse_line(t_parser *parser, char *line);
static int	check_validity_of_scene(t_parser *parser);
static int	transfer_lists_to_arrays(t_parser *parser, t_info *info);
static void	copy_light(t_parser *parser, t_info *info);
static void	copy_obj(t_type type, t_parser *parser, uint8_t *i, uint8_t n_obj);

void	parse_scene(t_info *info, char *filename)
{
	t_parser	parser;
	char		*line;
	t_status	status;

	line = NULL;
	status = NO_ERROR;

	// initialize parser struct
	ft_bzero(&parser, sizeof (t_parser));
	parser->line_num = 1;

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
					if (parser->line_num < UINT32_MAX) // just to avoid a 'potential' segfault
						parser->line_num++;
				}
			}
			else	// file has been fully read.
			{
				status = check_validity_of_scene(&parser);

				// transfer the lights and all objects linked lists into their respective arrays;
				if (status == NO_ERROR)
					status = transfer_lists_to_arrays(info, &parser);

				// destroy the lists and return (unless malloc() failure has occured,
				// in which case status is ALLOCATION_FAILURE, which will be handled after the loop.
				if (status == NO_ERROR)
				{
					// destroy both linked lists, and close the file descriptor
					if (clean_up_parser(parser, NULL) == CLOSE_FAILURE) // if true, close() failed, but memory has been freed. Time to exit.
						exit (SYSTEM_FAILURE);
					return ;
				}
			}
		}
	}
	if (status != NO_ERROR)
		exit(handle_parsing_error(status, line, &parser));
}

// FIXME: consider doing the isspace_but_not_newline() checks from the specific
// parsing functions, to make this look more clean -> and to go with the general
// "atoi()" type logic....
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
		return (parse_light(&parser, str + 2, parser));
	else if (*str == 's' && *(str + 1) == 'p'
		&& isspace_but_not_newline(*(str + 2)))
		return (parse_sphere(str + 3, parser->line_num));
	else if (*str == 'p' && *(str + 1) == 'l'
		&& isspace_but_not_newline(*(str + 2)))
		return (parse_plane(str + 3, parser->line_num));
	else if (*str == 'c' && *(str + 1) == 'y'
		&& isspace_but_not_newline(*(str + 2)))
		return (parse_cylinder(str + 3, parser->line_num));
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

static int	transfer_lists_to_arrays(t_parser *parser, t_info *info)
{
	uint8_t	i;

	// update n_light in 'info'
	info->n_light = parser->n_lights;

	// allocate array of lights in 'info'

	info->light = (t_light *) ft_calloc(parser->n_lights, sizeof (t_light));
	if (!info->light)
		return (ALLOCATION_FAILURE);


	// copy 'light' linked list data into the new 'light' array
	copy_light(parser, info);

	// update n_obj in 'info'
	info->n_obj = parser->spheres + parser->planes + parser->cylinders;

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

static void	copy_light(t_parser *parser, t_info *info)
{
	t_node_light	*current;
	uint32_t		i;

	current = parser->head;
	i = 0;
	while (current)
	{
		info->light = current->object->light;
		i++;
		current = current->next;
	}
}

static void	copy_obj(t_type type, t_parser *parser, uint8_t *i, uint8_t n_obj)
{
	t_info		*info;
	uint8_t		j;
	t_node_obj	*current;

	info = get_info();
	j = *i;
	current = parser->head_obj;

	while (current && n_obj)
	{
		if (current->object.type == type)
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
	if (!parser->n_lights)
	{
		ft_putstr_fd("Error\nScene description file has no light source.\n", 2);
		return (INVALID_INPUT);
	}
	if (!parser->n_cams)
	{
		ft_putstr_fd("Error\nIncomplete file provided: no camera found.\n", 2);
		return (INVALID_INPUT);
	}
	if (!(parser->n_spheres + parser->n_planes + parser->n_cylinders))
	{
		ft_putstr_fd("Error\nProvided scene has no objects. At least one "
			"object should be proposed for the scene to be rendered\n", 2);
		return (INVALID_INPUT);
	}
	return (NO_ERROR);
}
