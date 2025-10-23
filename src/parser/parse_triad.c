/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_triad.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 19:52:08 by ykadosh           #+#    #+#             */
/*   Updated: 2025/10/18 19:38:59 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	str_to_linear_color(char **str, double *result);

/*
* This function expects three color values in range 0 to 255, separated by
* commas. One space after a comma is also accepted; for example:
* "r,g,b" and "r, g, b" are both accepted inputs. If the input does not comply
* with those requirements, -1 is returned to indicate an error.
* If the input is valid, the function converts the values and stores the r,g,b
* values into the three members of 'color', in that order.
* At the end of the function, the caller's 'str' points one character past the
* last digit of the third value.
*/
int	parse_color(char **str, t_color *color, double ratio)
{
	if (str_to_linear_color(str, &color->r) == -1)
		return (-1);
	if (!is_valid_separator(str))
		return (-1);
	if (str_to_linear_color(str, &color->g) == -1)
		return (-1);
	if (!is_valid_separator(str))
		return (-1);
	if (str_to_linear_color(str, &color->b) == -1)
		return (-1);
	apply_ratio_to_color(color, ratio, 1);
	return (0);
}

static int	str_to_linear_color(char **str, double *result)
{
	char		*s;
	uint16_t	conversion;

	conversion = 0;
	s = *str;
	if (*s == '+')
		s++;
	if (!ft_isdigit(*s))
		return (-1);
	while (ft_isdigit(*s))
	{
		conversion = conversion * 10 + *s - '0';
		if (conversion > 255)
			return (-1);
		s++;
	}
	*result = (double) conversion / 255.0;
	*str = s;
	return (0);
}

/*
* In case where the RGB channels of 'color' are provided by the input file,
* is_provided should be passed as 1 or 'true', and this function then applies
* 'ratio' to those values (which should have been previously parsed and
* normalized by parse_color().
* Otherwise, is_provided should be 0 or 'false'. In this case, miniRT defaults
* to WHITE, and the normalized RGB values should therefore be set to 1.0. Since
* multiplying by 1.0 is redundant, this function simply assigns the ratio itself
* to each channel.
*/
void	apply_ratio_to_color(t_color *color, double ratio, bool is_provided);
{
	if (is_provided)
	{
		color->r = color->r * ratio;
		color->g = color->g * ratio;
		color->b = color->b * ratio;
	}
	else
	{
		color->r = ratio;
		color->g = ratio;
		color->b = ratio;
	}
}

int	parse_coordinates(char **str, t_vec *position, uint32_t line_num)
{
	if (ft_strtod(str, &position->x, line_num) == -1)
		return (-1);
	if (!is_valid_separator(str))
	{
		display_parsing_error("Unexpected input on line:", line_num);
		return (-1);
	}
	if (ft_strtod(str, &position->y, line_num) == -1)
		return (-1);
	if (!is_valid_separator(str))
	{
		display_parsing_error("Unexpected input on line:", line_num);
		return (-1);
	}
	if (ft_strtod(str, &position->z, line_num) == -1)
		return (-1);
	return (0);
}

int	parse_direction_vector(char **str, t_vec *direction, uint32_t line_num)
{
	if (ft_strtod(str, &direction->x, line_num) == -1)
		return (-1);
	if (!is_valid_separator(str))
	{
		display_parsing_error("Unexpected input on line:", line_num);
		return (-1);
	}
	if (ft_strtod(str, &position->y, line_num) == -1)
		return (-1);
	if (!is_valid_separator(str))
	{
		display_parsing_error("Unexpected input on line:", line_num);
		return (-1);
	}
	if (ft_strtod(str, &position->z, line_num) == -1)
		return (-1);
	return (0);
}
