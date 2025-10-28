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

void	parse_scene(t_info *info, char *filename)
{
	t_parser	parser;
	int			fd;
	char		*line;
	uint32_t	line_num; // always positive!
	int			error_code;

	line = NULL;
	line_num = 1;
	error_code = 0;

	// initialize parser struct
	ft_bzero(&parser, sizeof (t_parser));

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		error_code = -4;
	while (!error_code)
	{
		error_code = get_next_line_minirt(fd, &line);

		if (!error_code)
		{
			if (line)
			{
				error_code = parse_line(&parser, line, line_num);
				if (!error_code)
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


				// check that we have all data we need in order to execute rendering.
				// Else: set error_code to 1, display an error message (display_parsing_error()).
				// so that the program would exit

				// transfer the lights and all objects linked lists into their respective arrays;
				error_code = transfer_lists_to_arrays(info, &parser);

				// destroy the lists and return (unless malloc() failure has occured,
				// in which case error_code is -1, which will be handled after the loop.
				if (!error_code)
					return;
			}
		}
	}
	if (error_code)
		exit(handle_parsing_error(error_code, line, &parser));
}

// FIXME: consider doing the isspace_but_not_newline() checks from the specific
// parsing functions, to make this look more clean -> and to go with the general
// "atoi()" type logic....
static int	parse_line(t_parser *parser, char *line, uint32_t line_num)
{
	t_info	*info;
	char	*str;	// used to be able to free 'line'

	info = get_info();
	str = line;


	skip_whitespace_but_not_newline(&str);

	if (*str == '#' || *str == '\n')	// ignore comments in .rt file || line is 'empty' but valid
		return (0);
	else if (*str == 'A' && isspace_but_not_newline(*(str + 1)))
		return (parse_ambient_lighting(&info->amb, str + 2, line_num));
	else if (*str == 'C' && isspace_but_not_newline(*(str + 1)))
		return (parse_camera(&info->cam, str + 2, line_num));
	else if (*str == 'L' && isspace_but_not_newline(*(str + 1)))
		return (parse_light(&parser, str + 2, line_num));
	else if (*str == 's' && *(str + 1) == 'p'
		&& isspace_but_not_newline(*(str + 2)))
		return (parse_sphere(str + 3, line_num));
	else if (*str == 'p' && *(str + 1) == 'l'
		&& isspace_but_not_newline(*(str + 2)))
		return (parse_plane(str + 3, line_num));
	else if (*str == 'c' && *(str + 1) == 'y'
		&& isspace_but_not_newline(*(str + 2)))
		return (parse_cylinder(str + 3, line_num));
	// The next boolean check is important, because if it returns false, we have
	// an empty whitespace line ending with the EOF, which might simply be the
	// input file's very last line. That should not be considered as a parsing
	// error, and the program should proceed.
	if (*str)
	{
		err_code = 1;
		display_parsing_error("Unexpected input provided on line number",
			line_num);
		return (1);
	}
	return (0);
}

// TODO:
static int	transfer_lists_to_arrays(t_info *info, t_parser *parser)
{
	uint8_t	i;

	// update n_light in 'info'
	info->n_light = parser->n_lights;


	// allocate array of lights in 'info'
	// make ft_calloc() failure check!

	info->light = (t_light *) ft_calloc(parser->n_lights, sizeof (t_light));
	if (!info->light)
		return (-1);


	// FIXME: can we safely remove parser->current? Do I really need it?? have to review allocation in parse_light() first, it has to be refactored for us to know...
	// FIXME: refactor the following block. i can be an internal variable of that function, do not mix it with the above declared uint8_t i.
	t_node_light	*current;
	uint32_t		i;

	current = parser->head;
	i = 0;

	while (current)
	{
		info->light[i].pos.x = current->object.pos.x;
		info->light[i].pos.y = current->object.pos.y;
		info->light[i].pos.z = current->object.pos.z;

		info->light[i].color.x = current->object.color.x;
		info->light[i].color.y = current->object.color.y;
		info->light[i].color.z = current->object.color.z;
		i++;
		current = current->next;
	}
	// FIXME: end of refactoring block.





	// update n_obj in 'info'
	info->n_obj = parser->spheres + parser->planes + parser->cylinders;


	// allocate array of objects. make ft_calloc() failure check!
	info->obj = (t_object *) ft_calloc(info->n_obj, sizeof (t_object));
	if (!info->obj)
		return (-1);

	// TODO:
	// copy all spheres' data to the START of the array.

	i = 0;


	// TODO:
	// copy all planes' data to the MIDDLE of the array.
	// do NOT reset i, it keeps incrementing through the array.




	// TODO:
	// copy all cylinders' data to the END of the array.



	// destroy both linked lists
	clean_up_parsing_memory(parser, NULL);
}

// FIXME:
static int	transfer_light()
{
	while 


}


// FIXME: refactor.
static uint8_t	copy_obj_type_to_array(t_type type, t_parser *parser, uint8_t i)
{
	t_info		*info;
	uint8_t		n_objects;
	t_node_obj	*current;

	info = get_info();

	if (type == SPHERE)
		n_objects = parser->n_spheres;
	else if (type == PLANE)
		n_objects = parser->n_planes;
	else
		n_objects = parser->n_cylinders;

	current = parser->head_obj;
	while (n_objects)
	{
		while (current)
		{
			// FIXME: refactor the next block into a separate function !!!
			if (current->object.type == type)
			{
				// copy all data
				info->obj[i].type = type;

				info->obj[i].pos.x = current->object.pos.x;
				info->obj[i].pos.y = current->object.pos.y;
				info->obj[i].pos.z = current->object.pos.z;

				info->obj[i].color.x = current->object.color.x;
				info->obj[i].color.y = current->object.color.y;
				info->obj[i].color.z = current->object.color.z;

				if (type != PLANE)
					info->obj[i].r = current->object.r;

				if (type != SPHERE)
				{
					info->obj[i].normal.x = current->object.normal.x;
					info->obj[i].normal.y = current->object.normal.y;
					info->obj[i].normal.z = current->object.normal.z;
				}

				if (type == CYLINDER)
					info->obj[i].h = current->object.h;

				i++;
				n_objects--;
			}
			current = current->next;
		}
	}
	return (i);
}
