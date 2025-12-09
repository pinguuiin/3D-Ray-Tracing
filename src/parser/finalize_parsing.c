/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalize_parsing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:03:14 by ykadosh           #+#    #+#             */
/*   Updated: 2025/12/09 17:16:03 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	check_validity_of_scene(t_parser *parser);

#ifndef BONUS

static int	transfer_obj_list_to_array(t_parser *parser, t_info *info);
#else

static int	transfer_lists_to_arrays(t_parser *parser, t_info *info);
#endif

#ifndef BONUS

int	finalize_parsing(t_parser *parser, t_info *info)
{
	t_status	status;

	status = check_validity_of_scene(parser);
	if (status == NO_ERROR)
		status = transfer_obj_list_to_array(parser, info);
	if (status == NO_ERROR)
	{
		if (clean_up_parser(parser) == CLOSE_FAILURE)
			exit (SYSTEM_FAILURE);
	}
	return (status);
}

static int	transfer_obj_list_to_array(t_parser *parser, t_info *info)
{
	int	i;

	info->n_obj = parser->n_spheres + parser->n_planes + parser->n_cylinders;
	info->obj = (t_object *) ft_calloc(info->n_obj, sizeof (t_object));
	if (!info->obj)
		return (ALLOCATION_FAILURE);
	i = 0;
	copy_obj(SPHERE, parser, &i, parser->n_spheres);
	copy_obj(PLANE, parser, &i, parser->n_planes);
	copy_obj(CYLINDER, parser, &i, parser->n_cylinders);
	return (NO_ERROR);
}
#else

int	finalize_parsing(t_parser *parser, t_info *info)
{
	t_status	status;

	status = check_validity_of_scene(parser);
	if (status == NO_ERROR)
		status = transfer_lists_to_arrays(parser, info);
	if (status == NO_ERROR)
	{
		if (clean_up_parser(parser) == CLOSE_FAILURE)
			exit (SYSTEM_FAILURE);
	}
	return (status);
}

static int	transfer_lists_to_arrays(t_parser *parser, t_info *info)
{
	int	i;

	info->n_light = parser->n_lights;
	info->light = (t_light *) ft_calloc(parser->n_lights, sizeof (t_light));
	if (!info->light)
		return (ALLOCATION_FAILURE);
	copy_light(parser, info);
	info->n_obj = parser->n_spheres + parser->n_planes + parser->n_cylinders;
	info->obj = (t_object *) ft_calloc(info->n_obj, sizeof (t_object));
	if (!info->obj)
	{
		free (info->light);
		return (ALLOCATION_FAILURE);
	}
	i = 0;
	copy_obj(SPHERE, parser, &i, parser->n_spheres);
	copy_obj(PLANE, parser, &i, parser->n_planes);
	copy_obj(CYLINDER, parser, &i, parser->n_cylinders);
	return (NO_ERROR);
}
#endif

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
