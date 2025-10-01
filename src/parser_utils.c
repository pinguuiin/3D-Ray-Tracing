/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 18:15:18 by ykadosh           #+#    #+#             */
/*   Updated: 2025/09/30 18:20:25 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	parse_ambient_lighting(t_amb *amb, char *line)
{
	// TODO: first check if we already have ambient lighting: Only 1 is accepted
	if (amb)
		// free_exit(); only one ambien lighting element is accepted.
	

	line++;
	while (isspace_but_not_newline(*line))
		line++;


}

void	parse_camera(t_cam *cam, char *line)
{
	// TODO: first check if we already have a camera: Only 1 is accepted
	if (cam)

	line++;
	while (isspace_but_not_newline(*line))
		line++;

}

void	parse_light(t_light *light, char *line)
{
	// TODO: first check if we already have a light source: Only 1 is accepted
	if (light)


	line++;
	while (isspace_but_not_newline(*line))
		line++;

}
