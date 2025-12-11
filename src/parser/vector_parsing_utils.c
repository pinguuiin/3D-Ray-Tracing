/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_parsing_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 07:32:46 by ykadosh           #+#    #+#             */
/*   Updated: 2025/12/11 23:05:10 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool	is_within_range_vector(t_vec *vec, size_t line_num);

int	validate_vector(t_vec *vector, size_t line_num, t_vector_id id)
{
	if (id == SPHERE_AXIS)
		vector->z = 0.0;
	if (fabs(vector->x) < EPSILON && fabs(vector->y) < EPSILON
		&& fabs(vector->z) < EPSILON)
	{
		if (id == CAM_DIRECTION)
			display_parsing_error("Vector normalization failed: cannot accept "
				"a zero-length camera direction. Error on line", line_num);
		else if (id == PLANE_NORMAL)
			display_parsing_error("Vector normalization failed: cannot "
				"normalize a zero-length plane normal. See line:", line_num);
		else if (id == CYLINDER_AXIS)
			display_parsing_error("Vector normalization failed: cannot "
				"normalize a zero-length cylinder axis. See line:", line_num);
		else if (id == SPHERE_AXIS)
			display_parsing_error("Vector normalization failed: cannot accept "
				"a zero-length sphere axis.\n"
				"Please note that the z-component is ignored and parsed as 0.0 "
				"when the vector belongs to a sphere.\nSee line:", line_num);
		return (0);
	}
	*vector = normalize(*vector);
	return (1);
}

int	parse_and_normalize_vector(char **str, t_vec *vector, size_t line_num,
		t_vector_id id)
{
	if (parse_3d_vector(str, vector, line_num) == -1)
		return (-1);
	if (!is_within_range_vector(vector, line_num))
		return (-1);
	if (!validate_vector(vector, line_num, id))
		return (-1);
	if (!is_valid_tail_when_expecting_more_data(str, line_num))
		return (-1);
	return (0);
}

static bool	is_within_range_vector(t_vec *vec, size_t line_num)
{
	if (vec->x < -1.0 || vec->x > +1.0)
	{
		display_parsing_error("x axis of vector has to be within range "
			"[-1,+1] for miniRT to accept this file.\n"
			"This invalid input was detected on line number:", line_num);
		return (0);
	}
	if (vec->y < -1.0 || vec->y > +1.0)
	{
		display_parsing_error("y axis of vector has to be within range "
			"[-1,+1] for miniRT to accept this file.\n"
			"This invalid input was detected on line number:", line_num);
		return (0);
	}
	if (vec->z < -1.0 || vec->z > +1.0)
	{
		display_parsing_error("z axis of vector has to be within range "
			"[-1,+1] for miniRT to accept this file.\n"
			"This invalid input was detected on line number:", line_num);
		return (0);
	}
	return (1);
}
