/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 19:52:08 by ykadosh           #+#    #+#             */
/*   Updated: 2025/10/17 20:06:15 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int			str_to_linear_color(char **s, double *result);
static int			is_valid_separator(char	**s);

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
int	parse_color(char **str, t_color *color)
{
	if (str_to_linear_color(str, &color->r) == -1)
		return (-1);
	if (!is_valid_separator(str);
		return (-1);
	if (str_to_linear_color(str, &color->g) == -1)
		return (-1);
	if (!is_valid_separator(str);
		return (-1);
	if (str_to_linear_color(str, &color->b) == -1)
		return (-1);
	return (0);
}

static int	str_to_linear_color(char **s, double *result)
{
	char		*ptr;
	uint16_t	conversion;

	conversion = 0;
	ptr = *s;

	if (*ptr == '+')
		ptr++;
	if (!ft_isdigit(*ptr))
		return (-1);
	while (ft_isdigit(*ptr))
	{
		conversion = conversion * 10 + *ptr - '0';
		if (conversion > 255)
			return (-1);
		ptr++;
	}
	*result = (double) conversion / 255.0;
	*s = ptr;
	return (0);
}

static int	is_valid_separator(char	**s)
{
	if (**s == ',')
		(*s)++;
	else
		return (0);
	if (**s == ' ')
		(*s)++;
	return (1);
}
