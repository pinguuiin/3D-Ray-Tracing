/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:24:08 by ykadosh           #+#    #+#             */
/*   Updated: 2025/12/09 17:29:04 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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
