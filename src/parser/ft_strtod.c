/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtod.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 19:28:18 by ykadosh           #+#    #+#             */
/*   Updated: 2025/12/16 18:32:19 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int		parse_plus_or_minus_sign(char **str);
static int		convert_whole(char **str, double *result, size_t n_digits,
					size_t line_num);
static int64_t	extract_unsigned_integer(char **ptr, size_t *n_digits);
static double	extract_fraction(char **str, size_t n_digits);

/*
* 'result', which points at the principal output of this function - meaning the
* floating point value stored at 'str', has to point at a zero value before
* calling this function.
*/
int	ft_strtod(char **str, double *result, size_t line_num)
{
	int		sign;
	size_t	n_digits;

	n_digits = 0;
	sign = parse_plus_or_minus_sign(str);
	if (convert_whole(str, result, &n_digits, line_num) == -1)
		return (-1);
	*result += extract_fraction(str, n_digits);
	if (parse_exponent(str, n_digits, result, line_num) == -1)
		return (-1);
	*result *= sign;
	if (isinf(*result))
	{
		display_parsing_error("Overflow of real number provided on line",
			line_num);
		return (-1);
	}
	return (0);
}

/*
* returns -1 if the double pointer 'str' points a '-' sign, else returns +1.
* If 'str' points at either a '+' or '-' sign, it is incremented at the caller,
* to point at the next character.
*/
static int	parse_plus_or_minus_sign(char **str)
{
	int	sign;

	sign = 1;
	if (**str == '+')
		(*str)++;
	else if (**str == '-')
	{
		sign = -1;
		(*str)++;
	}
	return (sign);
}


// FIXME: Is the next case handled:
// if there are characters after the very last digit of the number in string form?
// This function seems to not handle that - so a check should always be made
// after this function returns,
// but it does not seem to be covered all the time. Double check.

static int	convert_whole(char **str, double *result, size_t n_digits,
				size_t line_num)
{
	int64_t	whole;

	if (ft_isdigit(**str))
	{
		whole = extract_unsigned_integer(str, &n_digits);
		if (whole == -1)
		{
			display_parsing_error("Floating point number provided is too "
				"large. See input file's line number", line_num);
			return (-1);
		}
		*result = (double) whole;
	}
	else if (**str == '.' && ft_isdigit(*str)[1])
		return (0);
	else
	{
		display_parsing_error("Unknown input when expecting floating point "
			"number, on line:", line_num);
		return (-1);
	}
	return (0);
}

static int64_t	extract_unsigned_integer(char **str, size_t *n_digits)
{
	char	*s;
	int64_t	temp;

	s = *str;
	temp = 0;
	while (ft_isdigit(*s))
	{
		temp = temp * 10.0 + (*s - '0');
		if (temp > UINT32_MAX)
			return (-1);
		(*n_digits)++;
		s++;
	}
	*str = s;
	return (temp);
}

static double	extract_fraction(char **str, size_t n_digits)
{
	double	fraction;
	size_t	i;

	fraction = 0.0;
	i = 0;
	if (**str == '.')
	{
		(*str)++;
		while (ft_isdigit(**str))
		{
			fraction = fraction * 10.0 + (**str - '0');
			i++;
			(*str)++;
			if (i >= 15 || n_digits + i >= 15)
			{
				if (**str >= '5' && **str <= '9')
					fraction++;
				while (ft_isdigit(**str))
					(*str)++;
			}
		}
		// FIXME: double check if those next blocks are identical!
		while (i-- > 0)
			fraction *= 0.1;
		/*
		while (i)
		{
			fraction *= 0.1;
			i--;
		}
		*/
	}
	return (fraction);
}
