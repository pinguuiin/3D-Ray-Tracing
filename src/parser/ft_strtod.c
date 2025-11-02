/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtod.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 21:13:24 by ykadosh           #+#    #+#             */
/*   Updated: 2025/10/18 19:49:09 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int				parse_plus_or_minus_sign(char **ptr);
static bool				is_start_of_string_valid(const char *s);
static inline int64_t	extract_unsigned_integer(char **ptr, size_t *n_digits);
static inline double	extract_fraction(char **ptr, size_t n_digits);
static inline int		extract_exponent_and_update_result(char **ptr,
						size_t n_digits, double *result);

int	ft_strtod(char **str, double *result, uint32_t line_num)
{
	char	*ptr;
	int		sign;
	int64_t	whole;
	size_t	n_digits;

	ptr = *str;
	n_digits = 0;
	sign = parse_plus_or_minus_sign(&ptr);
	if (!is_start_of_string_valid(ptr))
	{
		display_parsing_error("Unknown input when expecting floating point "
			"number, on line:", line_num);
		return (-1);
	}

	if (ft_isdigit(*ptr))
	{
		whole = extract_unsigned_integer(&ptr, &n_digits);
		if (whole == -1)
		{
			display_parsing_error("Floating point number provided is too "
				"large. See input file's line number", line_num);
			return (-1);
		}
		*result = (double) whole;
	}

	// check for the radix point
	if (*ptr == '.')
	{
		ptr++;
		*result += extract_fraction(&ptr, n_digits);
	}
	if (*ptr == 'e' || *ptr == 'E')
	{
		ptr++;
		if (ft_isdigit(*ptr)
			|| ((*ptr == '+' || *ptr == '-') && ft_isdigit(*(ptr + 1))))
		{
			if (extract_exponent_and_update_result(&ptr, n_digits, result) == -1)
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

	*result *= sign;	// convert result to negative, if necessary
	if (isinf(*result))
	{
		display_parsing_error("Overflow of real number provided on line",
			line_num);
		return (-1);
	}
	*str = ptr;	// increments the pointer at the caller.
	return (0);
}

/*
* returns -1 if the double pointer 'ptr' points a '-' sign, else returns +1.
* If 'ptr' points at either a '+' or '-' sign, it is incremented at the caller,
* to point at the next character.
*/
static int	parse_plus_or_minus_sign(char **ptr)
{
	int	sign;

	sign = 1;
	if (**ptr == '+')
		(*ptr)++;
	else if (**ptr == '-')
	{
		sign = -1;
		(*ptr)++;
	}
	return (sign);
}

static bool	is_start_of_string_valid(const char *s)
{
	if (ft_isdigit(*s))
		return (1);
	else if (*s == '.')
	{
		if (ft_isdigit(*(s + 1)))
			return (1);
	}
	return (0);
}

// version which caps whole number to UINT32_MAX, counting n of digits
static inline int64_t	extract_unsigned_integer(char **ptr, size_t *n_digits)
{
	char	*s;
	int64_t	temp;

	s = *ptr;
	temp = 0;
	while (ft_isdigit(*s))
	{
		temp = temp * 10.0 + (*s - '0');
		if (temp > UINT32_MAX)
			return (-1);
		(*n_digits)++;
		s++;
	}
	*ptr = s;
	return (temp);
}

// takes into account n_digits of the integer part, to round up the last digit
// that would fit into 15 total digits for the eventual double.
static inline double	extract_fraction(char **ptr, size_t n_digits)
{
	double	fraction;
	char	*s;
	size_t	i;

	fraction = 0.0;
	s = *ptr;
	i = 0;
	while (ft_isdigit(*s))
	{
		fraction = fraction * 10.0 + (*s - '0');
		i++;
		s++;
		if (i >= 15 || n_digits + i >= 15)
		{
			if (*s >= '5' && *s <= '9')
				fraction++;
			while (ft_isdigit(*s))
				s++;
		}
	}
	while (i)
	{
		fraction *= 0.1;
		i--;
	}
	*ptr = s;
	return (fraction);
}

// This function only accepts a value whose order of magnitude is between
// -15 and +15. This is done in order to avoid imprecisions that can happen
// when the double is longer than 15 digits in total, and also to avoid overflow
static inline int	extract_exponent_and_update_result(char **ptr,
						size_t n_digits, double *result)
{
	char	*s;
	int		sign;
	int64_t	exponent;
	double	factor;

	s = *ptr;
	sign = 1;
	exponent = 0;
	if (*s == '+')
		s++;
	else if (*s == '-')
	{
		sign = -1;
		s++;
	}

	while (ft_isdigit(*s))
	{
		exponent = exponent * 10 + (*s - '0');
		if (exponent * sign < -15 || exponent * sign > 15 - (int64_t) n_digits)
			return (-1);
		s++;
	}
	*ptr = s;

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
	*result *= factor;
	return (0);
}
