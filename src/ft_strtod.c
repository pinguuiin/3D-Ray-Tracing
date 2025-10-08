/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtod.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 21:13:24 by ykadosh           #+#    #+#             */
/*   Updated: 2025/10/07 21:51:00 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

static int			parse_plus_or_minus_sign(char **str);
static inline int	extract_whole_positive_nbr(char **ptr, double *result);
static inline int	extract_fractional_part(char **ptr, double *result);

// TODO:
// WARN: consider error handling
// TEST: I handled each and every of the following cases, but it needs testing:
// "-", "- ", "+", "+ ", ".", ". ", "+.", "+. ", "-.", "-. ", "+.k334", "-.%43"
// FIXME: How many digits should I accept on the right of the decimal point?
// And if it is very long: should I trim the number, or should I return an error?
inline int	ft_strtod(char **str, double *result)
{
	char	*ptr;	// for readability.
	int		is_neg;

	ptr = *str;
	// WARN: is 'result' already set to zero when this function is called?
	// Should I set it to zero here, if not? It better be set to zero beforehand!

	is_neg = parse_plus_or_minus_sign(&ptr);
	if (is_neg == -1)
		return (-1);


	if (*ptr >= '0' && *ptr <= '9')
	{
		if (extract_whole_positive_nbr(&ptr, result) == -1)
		{
			// TODO: handle error
			// return (-1);
		}
	}

	// check for the decimal point
	if (*ptr == '.')
	{
		ptr++;
		if (extract_fractional_part(ptr, result) == -1)
		{
			// TODO: handle error
			// return (-1);
		}
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

	// convert result to negative, if it is prefixed by a negative sign
	if (is_neg)
		*result *= -1;


	// WARN: use math.h's 'isinf()' function, and perhaps also 'isnan()', to
	// check for the overflow of your double, before returning.
	// How should you handle the error, however, if it did overflow?

	// check that the acquired 'result' is not an overflow.
	if (isnan(*result) || isinf(*result))
	{
		// TODO: handle the error
		// return (-1);

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
static int	parse_plus_or_minus_sign(char **ptr)
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
	}
	(*ptr)++;
	return (is_neg);
}

static inline int	extract_whole_positive_nbr(char **ptr, double *result)
{
	char	*s;

	s = *ptr;
	while (*s >= '0' && *s <= '9')
	{
		



	}




	return (0);
}

static inline int	extract_fractional_part(char **ptr, double *result)
{


}
