/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exponent.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 00:38:48 by ykadosh           #+#    #+#             */
/*   Updated: 2025/12/16 00:38:58 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int		extract_exponent_and_update_result(char **str, size_t n_digits,
					double *result);
static double	calculate_exponent_factor(int64_t exponent, int sign);

int	parse_exponent(char **str, size_t n_digits, double *result, size_t line_num)
{
	if (**str == 'e' || **str == 'E')
	{
		(*str)++;
		if (ft_isdigit(**str)
			|| ((**str == '+' || **str == '-') && ft_isdigit((*str)[1])))
		{
			if (extract_exponent_and_update_result(str, n_digits, result) == -1)
			{
				display_parsing_error("Floating point number provided is "
					"either too large or too small;\nPlease provide a value "
					"within orders of magnitude -15 and +15. See line:",
					line_num);
				return (-1);
			}
		}
		else
		{
			display_parsing_error("Unknown input when expecting floating "
				"point number, on line:", line_num);
			return (-1);
		}
	}
	return (0);
}

static int	extract_exponent_and_update_result(char **str, size_t n_digits,
				double *result)
{
	int		sign;
	int64_t	exponent;

	sign = 1;
	exponent = 0;
	if (**str == '+')
		(*str)++;
	else if (**str == '-')
	{
		sign = -1;
		(*str)++;
	}

	while (ft_isdigit(**str))
	{
		exponent = exponent * 10 + (**str - '0');
		if (exponent * sign < -15 || exponent * sign > 15 - (int64_t) n_digits)
			return (-1);
		(*str)++;
	}
	*result *= calculate_exponent_factor(exponent, sign);
	return (0);
}

static double	calculate_exponent_factor(int64_t exponent, int sign)
{
	double	factor;

	factor = 1.0;
	if (sign == -1)
	{
		while (exponent)
		{
			factor *= 0.1;
			exponent--;
		}
	}
	else
	{
		while (exponent)
		{
			factor *= 10.0;
			exponent--;
		}
	}
	return (facotr);
}
