/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_and_obj_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:24:08 by ykadosh           #+#    #+#             */
/*   Updated: 2025/12/09 17:45:45 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	create_new_object_node(t_parser *parser)
{
	t_node_obj	*new_node;

	new_node = (t_node_obj *) ft_calloc(1, sizeof (t_node_obj));
	if (!new_node)
		return (-1);
	if (!parser->head)
		parser->head = new_node;
	else
		parser->current->next = new_node;
	parser->current = new_node;
	return (0);
}

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

#ifndef BONUS
#else

int	create_new_light_node(t_parser *parser)
{
	t_node_light	*new_node;

	new_node = (t_node_light *) ft_calloc(1, sizeof (t_node_light));
	if (!new_node)
		return (-1);
	if (!parser->head_light)
		parser->head_light = new_node;
	else
		parser->curr_light->next = new_node;
	parser->curr_light = new_node;
	return (0);
}

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
