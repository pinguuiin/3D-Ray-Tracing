/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 04:37:13 by piyu              #+#    #+#             */
/*   Updated: 2026/01/02 05:25:33 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#ifndef BONUS
#else

static void	assign_other_texture(t_object *object, size_t len, char *str)
{
	if (len == 5 && !ft_strncmp(str, "metal", 5))
	{
		object->ks = 0.9;
		object->kd = 0.4;
		object->shininess = 80;
	}
	else if (len == 4 && !ft_strncmp(str, "snow", 4))
	{
		object->ks = 0.02;
		object->kd = 0.8;
		object->shininess = 10;
	}
	else
	{
		object->ks = 0.2;
		object->kd = 0.8;
	}
	object->material = TEXTURE;
}

int	assign_material_param(t_object *object, size_t len, char *str)
{
	if (len == 7 && !ft_strncmp(str, "checker", 7))
	{
		object->material = CHECKER;
		return (0);
	}
	if (len == 3 && !ft_strncmp(str, "ice", 3))
	{
		object->material = ICE;
		object->ks = 0.4;
		object->kd = 0.7;
		object->shininess = 300;
	}
	else
		assign_other_texture(object, len, str);
	return (1);
}
#endif
