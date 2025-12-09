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

static int	init_parser(t_parser *parser, char *filename);
static void	prepare_next_line(t_parser *parser);
static int	parse_line(t_parser *parser, char *str, t_info *info);

void	parse_scene(t_info *info, char *filename)
{
	t_parser	parser;
	t_status	status;

	status = init_parser(&parser, filename);
	while (status == NO_ERROR)
	{
		status = get_next_line_revised(parser.fd, &parser.line);
		if (status == NO_ERROR)
		{
			if (parser.line)
			{
				status = parse_line(&parser, parser.line, info);
				if (status == NO_ERROR)
					prepare_next_line(&parser);
			}
			else
			{
				status = finalize_parsing(&parser, info);
				if (status == NO_ERROR)
					return ;
			}
		}
	}
	if (status != NO_ERROR)
		exit(handle_parsing_error(status, &parser));
}

static int	init_parser(t_parser *parser, char *filename)
{
	parser->line = NULL;
	ft_bzero(parser, sizeof (t_parser));
	parser->line_num = 1;
	parser->fd = open(filename, O_RDONLY);
	if (parser->fd == -1)
		return (OPEN_FAILURE);
	return (NO_ERROR);
}

static void	prepare_next_line(t_parser *parser)
{
	free(parser->line);
	parser->line = NULL;
	if (parser->line_num < SIZE_MAX)
		parser->line_num++;
}

#ifndef BONUS

/*
* Parses the given line 'str' according to the expected miniRT input.
* This function also accepts empty lines in the file, whitespaces between
* elements, and also commented lines - provided they begin with a '#'.
*
* The very last check for a non null terminator character pointed to by 'str',
* which has possibly moved ahead by that point as it is passed at the start as
* a double pointer to skip_whitespace_but_not_newline(), is very important:
* If it returns true, it implies that there is unexpected input in the file.
* If that check returns false, however, it means that the line has had some
* whitespace within it, and that it does not end with a newline - i.e., it is
* most probably the very last line of the file but should be considered as an
* empty line - therefore as valid input.
*/
static int	parse_line(t_parser *parser, char *str, t_info *info)
{
	skip_whitespace_but_not_newline(&str);
	if (*str == '#' || *str == '\n')
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
	if (*str)
	{
		display_parsing_error("Unexpected input provided on line number",
			parser->line_num);
		return (INVALID_INPUT);
	}
	return (NO_ERROR);
}
#else

static int	parse_line(t_parser *parser, char *str, t_info *info)
{
	skip_whitespace_but_not_newline(&str);
	if (*str == '#' || *str == '\n')
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
#else

void	copy_light(t_parser *parser, t_info *info)
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

void	copy_obj(t_type id, t_parser *parser, int *i, int n_obj)
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
			info->obj[j] = current->object;
			j++;
			n_obj--;
		}
		current = current->next;
	}
	*i = j;
}
