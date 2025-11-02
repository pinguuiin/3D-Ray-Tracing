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

#include "parser.h"

static int	str_to_linear_color(char **str, double *result, uint32_t line_num);

/*
* This function expects three color values in range 0 to 255, separated by
* commas. One space after each comma is also accepted, for example:
* "r,g,b" and "r, g, b" are both accepted inputs.
* If the input does not comply with those requirements, an error message is
* displayed on the standard error and -1 is returned.
* In case of valid input, the function converts the values, normalizes them into
* [0.0,1.0] range, and then multiplies them by the value pointed at by 'ratio'
* (if 'ratio' is not NULL). The resulting normalized r,g,b values are then
* stored into the three members of 'color', in that order.
* After returning from this function, the caller's 'str' points one character
* past the last digit of the third value.
*/
int	parse_color(char **str, t_color *color, double *ratio, uint32_t line_num)
{
	if (str_to_linear_color(str, &color->r, line_num) == -1)
		return (-1);
	if (!is_valid_separator(str, line_num))
		return (-1);
	if (str_to_linear_color(str, &color->g, line_num) == -1)
		return (-1);
	if (!is_valid_separator(str, line_num))
		return (-1);
	if (str_to_linear_color(str, &color->b, line_num) == -1)
		return (-1);
	if (ratio)
		apply_ratio_to_color(color, *ratio, 1);
	return (0);
}

static int	str_to_linear_color(char **str, double *result, uint32_t line_num)
{
	char		*s;
	uint16_t	conversion;

	conversion = 0;
	s = *str;
	if (*s == '+')
		s++;
	if (!ft_isdigit(*s))
	{
		display_parsing_error("Invalid input provided for color value. "
			"Please use integers between 0 and 255. See line", line_num);
		return (-1);
	}
	while (ft_isdigit(*s))
	{
		conversion = conversion * 10 + *s - '0';
		if (conversion > 255)
		{
			display_parsing_error("Invalid input provided for color value. "
				"Please use integers between 0 and 255. See line", line_num);
			return (-1);
		}
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
inline void	apply_ratio_to_color(t_color *color, double ratio, bool is_provided)
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

int	parse_3d_vector(char **str, t_vec *vector, uint32_t line_num)
{
	if (ft_strtod(str, &vector->x, line_num) == -1)
		return (-1);
	if (!is_valid_separator(str, line_num))
		return (-1);
	if (ft_strtod(str, &vector->y, line_num) == -1)
		return (-1);
	if (!is_valid_separator(str, line_num))
		return (-1);
	if (ft_strtod(str, &vector->z, line_num) == -1)
		return (-1);
	return (0);
}
