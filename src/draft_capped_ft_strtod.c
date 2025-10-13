/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draft_capped_ft_strtod.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 21:13:24 by ykadosh           #+#    #+#             */
/*   Updated: 2025/10/13 20:46:13 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

static int				parse_plus_or_minus_sign(char **ptr);
static int				is_start_of_string_valid(const char *s);
static inline double	extract_positive_integer_part(char **ptr);
static inline int		extract_fractional_part(char **ptr);
static inline int		extract_exponent_and_update_result(char **ptr, double *result);

// TODO: still incomplete. Consider making a true copy of stdlib's strtod()!
// TODO: Implement acceptance of "E" or "e" in the string, in cases where there
// is valid scientific notation. That character should be preceded by either:
// - at least one digit
// - a radix point "." -> if that radix point was preceded by at least 1 digit.
// TEST: I handled each and every of the following cases, but it needs testing:
// "-", "- ", "+", "+ ", ".", ". ", "+.", "+. ", "-.", "-. ", "+.k334", "-.%43"
// FIXME: How many digits should I accept on the right of the decimal point?
// And if it is very long: should I trim the number, or should I return an error?
inline int	ft_strtod(char **str, double *result)
{
	char	*ptr;	// for readability.
	int		sign;

	ptr = *str;
	// WARN: is 'result' already set to zero when this function is called?
	// Should I set it to zero here, if not? It better be set to zero beforehand!

	sign = parse_plus_or_minus_sign(&ptr);
	if (!is_start_of_string_valid(ptr))
	{
		display_parsing_error("Unknown input when expecting floating point "
			"number, on line:", line_num);
		return (-1);
	}

	if (ft_isdigit(*ptr))
	{
		*result = extract_positive_integer_part(&ptr);
		if (isinf(*result))
		{
			// TODO: handle error
			display_parsing_error("Overflow of floating point number has "
			"occured. Please provide a different value, on line", line_num);
			return (-1);
		}
	}

	// check for the radix point
	if (*ptr == '.')
	{
		ptr++;
		// WARN: extract_fractional_part() still unsafe.
		*result += extract_fractional_part(&ptr);

		/*
		if (extract_fractional_part(&ptr, result) == -1)
		{
			// TODO: handle error
			// display_parsing_error("??????", line_num);
			// return (-1);
		}
		*/
	}
	if (*ptr == 'e' || *ptr == 'E')
	{
		ptr++;
		if (!ft_isdigit(*ptr)
			|| ((*ptr == '+' || *ptr == '-') && !ft_isdigit(*(ptr + 1))))
		{
			display_parsing_error("Unknown input when expecting floating point "
				"number, on line:", line_num);
			return (-1);
		}
		// TODO:
		if (extract_exponent_and_update_result(&ptr, result) == -1)
		{
			display_parsing_error("Unknown input when expecting floating point "
				"number, on line:", line_num);
			return (-1);
		}
	}

	// check that the number has no strange tail
	// accept whitespace (including '\n' & nul terminator)
	if (*str && !ft_isspace(*str))
	{
		// NOTE: Add check for the '\n' again, as well as for '\0' in the
		// caller, after this function returns -> because I think strtod()
		// should accept those as valid tails for the floating point value,
		// as a general rule, but in the context of miniRT, depending on the
		// element we are parsing in the caller: this could be an error.

		display_parsing_error("Unknown input when expecting floating point "
			"number, on line:", line_num);
		return (-1);
	}

	// convert result to negative, if necessary
	*result *= sign;

	// check that the acquired 'result' is not an overflow.
	if (isnan(*result) || isinf(*result)) // WARN: isnan() might be overkill...
	{
		display_parsing_error("Overflow of floating point number has occured. "
		"Please provide a different value, on line", line_num);
		return (-1);

	}

	// set the caller's string pointer at the same place
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

static int	is_start_of_string_valid(const char *s)
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

// TODO: think about how many digits you accept - on both hands of the
// decimal point? while ensuring maximal precision of the provided number.
static inline double	extract_positive_integer_part(char **ptr)
{
	double	result;
	char	*s;

	result = 0.0;
	s = *ptr;
	while (ft_isdigit(*s))
	{
		result = result * 10.0 + (*s - '0');
		s++;
	}
	*ptr = s;
	return (result);
}

// TODO: think about how many digits you accept - on both hands of the
// decimal point? while ensuring maximal precision of the provided number.
// TODO: should I accept exponents "e" / "E", scientific notation?
static inline int	extract_fractional_part(char **ptr)
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
	}

	if (fraction)
		fraction = 10.0 * i / fraction;
	*ptr = s;
	return (fraction);
}

static inline int	extract_exponent_and_update_result(char **ptr, double *result)
{
	char	*s;
	int		is_neg;
	int64_t	exponent;
	double	temp;
	

	s = *ptr;
	exponent = 0;
	is_neg= 0;
	if (s == '+')
		s++;
	else if (s == '-')
	{
		is_neg = 1;
		s++;
	}

	while (ft_isdigit(*s))
	{
		exponent = exponent * 10 + (*s - '0');
		s++;
		if (is_neg)
			exponent *= -1;
		if (exponent > INT_MAX || exponent < INT_MIN)
			return (-1);
	}
	*ptr = s;

	temp = 1.0;
	if (is_neg)
	{
		while (exponent)
		{
			temp *= 0.1;
			exponent++;
		}
		temp = -temp;
	}
	else
	{
		while (exponent)
		{
			temp *= 10.0;
			exponent--;
		}
	}
	*result *= temp;
	/*
	if (isinf(*result) || isnan(*result))
		return (-1);
	*/
	return (0);
}
