/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 07:32:46 by ykadosh           #+#    #+#             */
/*   Updated: 2025/10/03 19:27:00 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	create_new_object_node(t_parser *parser);

// NOTE: In all functions of parse_setting.c and parse_objects.c:
// The pointer 'str' is always pointing one byte past the scene's type
// identifier ('A', 'C', 'L', "sp", "pl" or "cy") AND the whitespace
// (non-newline) character that follows it!
int	parse_sphere(t_parser *parser, char *str, uint32_t line_num)
{
	t_object	*sphere;

	sphere = NULL;
	if (create_new_object_node(parser) == -1)
		return (-1);
	sphere = &parser->curr_obj->object;


	skip_whitespace_but_not_newline(&str);





	parser->n_objs++;
	return (0);
}

int	parse_plane(t_parser *parser, char *str, uint32_t line_num)
{
	t_object	*plane;

	plane = NULL;
	if (create_new_object_node(parser) == -1)
		return (-1);
	plane = &parser->curr_obj->object;




	skip_whitespace_but_not_newline(&str);






	parser->n_objs++;
	return (0);
}

int	parse_cylinder(t_parser *parser, char *str, uint32_t line_num)
{
	t_object	*cylinder;

	cylinder = NULL;
	if (create_new_object_node(parser) == -1)
		return (-1);
	cylinder = &parser->curr_obj->object;



	skip_whitespace_but_not_newline(&str);












	parser->n_objs++;
	return (0);
}

static int	create_new_object_node(t_parser *parser)
{
	if (!parser->head_obj)
	{
		parser->head_obj = (t_node_obj *) ft_calloc(1, sizeof (t_node_obj));
		if (!parser->head_obj)
			return (-1);
		parser->curr_obj = parser->head_obj;
	}
	else
	{
		parser->curr_obj->next = (t_node_obj *) ft_calloc(1,
			sizeof (t_node_obj));
		if (!parser->curr_obj->next)
			return (-1);
		parser->curr_obj = parser->curr_obj->next;
	}
	return (0);
}
