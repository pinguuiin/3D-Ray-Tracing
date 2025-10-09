/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temporary_draft_2_ft_strtod.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 21:13:24 by ykadosh           #+#    #+#             */
/*   Updated: 2025/10/09 18:02:31 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

static int				is_start_of_string_valid(char *s);
static inline double	extract_positive_integer_part(char **ptr);
static inline int		extract_fractional_part(char **ptr);

// TODO: still incomplete. Consider making a true copy of stdlib's strtod()!
// TEST: I handled each and every of the following cases, but it needs testing:
// "-", "- ", "+", "+ ", ".", ". ", "+.", "+. ", "-.", "-. ", "+.k334", "-.%43"
// FIXME: How many digits should I accept on the right of the decimal point?
// And if it is very long: should I trim the number, or should I return an error?
inline int	ft_strtod(char **str, double *result)
{
	char	*ptr;	// for readability.
	int		is_exponent;
	int		is_neg;

	ptr = *str;
	// WARN: is 'result' already set to zero when this function is called?
	// Should I set it to zero here, if not? It better be set to zero beforehand!

	is_exponent = 0;
	is_neg = 0;
	if (*ptr == '+' || *ptr == '-')
	{
		if (*ptr == '-')
			is_neg = 1;
		ptr++;
	}
	if (!is_start_of_string_valid(ptr))
	{
		display_parsing_error("????", line_num); // TODO: write message?
		return (-1);
	}


	if (*ptr >= '0' && *ptr <= '9')
	{
		*result = extract_positive_integer_part(&ptr);
		if (isinf(*result))
		{
			// TODO: handle error
			// display_parsing_error("Overflow of floating point number has "
			// "occured. Please provide a different value, on line", line_num);
			return (-1);
		}
	 // after 'e' or 'E', ONLY parse the exponent (handing invalid input) and return.
		// before these characters, you HAVE to have had at least a digit!
		if (*ptr == 'e' || *ptr == 'E')
		{
			is_exponent = 1;
			ptr++;

		}
	}
	else if (*ptr == 'e' || *ptr == 'E')
	{
		display_parsing_error(); // TODO:
		return (-1);
	}

	// WARN: handle case where you would arrive here and *ptr is still pointing
	// at the very first character - then, this exponent should not be taken care of,
	// it is invalid input. Consider checking for it at the caller?
	if (*ptr == 'e' || *ptr == 'E')
	{
		is_exponent = 1;


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

	// check that the number has no strange tail
	// accept whitespace (including '\n' & nul terminator)
	// Also, if we get here and the number has not been valid, for example:
	// "a13.4", we will
	if (*str && !ft_isspace(*str))
	{
		// TODO: handle error - first character after the very last digit of the
		// floating point number is unaccepted.
		// NOTE: Add check for the '\n' again, as well as for '\0' in the
		// caller, after this function returns -> because I think strtod()
		// should accept those as valid tails for the floating point value,
		// as a general rule, but in the context of miniRT, depending on the
		// element we are parsing in the caller: this could be an error.

		return (-1);
	}

	// convert result to negative if necessary
	if (is_neg)
		*result *= -1;	// no worries, overflow of double cannot happen here,
						//unless the value already overflowed.


	// check that the acquired 'result' is not an overflow.
	if (isnan(*result) || isinf(*result)) // WARN: isnan() might be overkill...
	{
		display_parsing_error("????", line_num); // TODO: error message?
		return (-1);

	}

	// set the caller's string pointer at the same place
	*str = ptr;	// increments the pointer at the caller.
	return (0);
}

/*
* a '-' or '+' sign which is NOT followed by either:
* 1. a digit
* 2. a decimal point which is in turn followed by a digit
* is considered invalid input.
*
* Checks if the character that ptr points at is a '+' or a '-' sign, and, if it
* is, checks the validity of the head of the value that follows it.
* Return values:
* • 0 : If no negative/ positive sign was found, or if a '+' sign was found and
* the start of the value seems valid.
* • 1 : If 'ptr' points at a negative sign, and the head of the value that
* follows appears valid.
* • -1 : If a sign was found, but the value is invalid.
*
* For example: "-.34" and "+.5" are considered valid values and would ultimately
* be interpreted as "-0.34" and "0.5", respectively. But examples such as "-",
* "+ ", "-.", "+.  " and "+.y23" are all treated as invalid by this function.
*/
// FIXME: this function should already get rid of inputs such as :
// ".  ", ".", ".s" or ".\n". Consider renaming it if you add this cleanup.
// Or, make a new helper that will check for that error, and use it in the currently
// available if statement following '-' or '+'.
// Do NOT use it however in the radix point check which is in ft_strtod() after
// extract_integer_part() -> since I'd like to accept inputs such as "12."
// NOTE: previous version without exponents
// static int	parse_plus_or_minus_sign(char **ptr)
{
	int		is_neg;
	char	*s;

	is_neg = 0;
	s = *ptr;
	if (*s == '-' || *s == '+')
	{
		s++;
		if ((*s >= '0' && *s <= '9')
			|| (*s == '.' && *(s + 1) >= '0' && *(s + 1) <= '9'))
		{
			if (*(s - 1) == '-')
				is_neg = 1;
		}
		else
			is_neg = -1;
		(*ptr)++;
	}
	return (is_neg);
}


// FIXME: this function should already get rid of inputs such as :
// ".  ", ".", ".s" or ".\n". Consider renaming it if you add this cleanup.
// Or, make a new helper that will check for that error, and use it in the currently
// available if statement following '-' or '+'.
// Do NOT use it however in the radix point check which is in ft_strtod() after
// extract_integer_part() -> since I'd like to accept inputs such as "12."
// NOTE: draft with exponents
static int	parse_plus_or_minus_sign(char **ptr)
{
	int		is_neg;
	char	*s;

	is_neg = 0;
	s = *ptr;
	if (*s == '-' || *s == '+')
	{
		s++;
		if (ft_isdigit(*s) || (*s == '.' && ft_isdigit(*(s + 1))))
		{
			if (*(s - 1) == '-')
				is_neg = 1;
		}
		else
			is_neg = -1;
		(*ptr)++;
	}
	return (is_neg);
}

/*
static int	parse_start_of_string_and_evaluate_if_pos_or_neg(char **ptr)
{
	int		is_neg;
	char	*s;

	is_neg = 0;
	s = *ptr;
	if (*s == '.')
	{
		if (!ft_isdigit(*(s + 1)))
			return (-1); // have to return because 
	}
	else if (*s == '-' || *s == '+')
	{
		s++;
		if (ft_isdigit(*s) || (*s == '.' && ft_isdigit(*(s + 1))))
		{
			if (*(s - 1) == '-')
				is_neg = 1;
		}
		else
			is_neg = -1;
	}
	(*ptr)++;
	return (is_neg);
}
*/

// WARN: review this function!
static int	is_start_of_string_valid(char *s)
{
	if (*s == '.')
	{
		if (ft_isdigit(*(s + 1)))
			return (1);
	}
	else if (ft_isdigit(*s))
		return (1);
	return (0);
}

static void	is_valid_radix_point(char *p_str)
{
	if (*p_str == '.')
	{
		p_str++;
		if (*p_str >= '0' && *p_str <= '9')
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
	while (*s >= '0' && *s <= '9')
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
	while (*s >= '0' && *s <= '9')
	{
		fraction = fraction * 10.0 + (*s - '0');
		i++;
	}

	if (fraction)
		fraction = 10.0 * i / fraction;
	*ptr = s;
	return (fraction);
}
