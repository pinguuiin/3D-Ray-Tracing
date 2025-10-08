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

static inline int	extract_whole_nbr_positive(char **ptr, double *whole_nbr);
static inline int	extract_fractional_part(char **ptr, double *fraction);

// TODO:
// WARN: consider error handling
inline int	ft_strtod(char **str, double *result)
{
	char	*ptr;	// for readability.
	int		is_neg;

	ptr = *str;
	is_neg = 0;
	// NOTE: is 'result' already set to zero when this function is called?
	// Should I set it to zero here, if not? It better be set to zero beforehand.

	if (*ptr == '-')
	{
		is_neg = 1;
		ptr++;
	}
	else if (*ptr == '+')
		ptr++;
	if (*ptr >= '0' && *ptr <= '9')
	{
		if (extract_whole_nbr(ptr, result) == -1)
		{
			// TODO: handle the error
			// return
		}
	}





	// TODO: parse the floating point '.'
	if (*ptr == '.')
	{

		if (extract_fractional_part(ptr, result) == -1)

	}


	// WARN: use math.h's 'isinf()' function, and perhaps also 'isnan()', to
	// check for the overflow of your double, before returning.
	// How should you handle the error, however, if it did overflow?

	if (isnan(*result) || isinf(*result))
	{
		// TODO: handle the error

	}

	*str = ptr;	// increments the pointer at the caller.
}

static inline int	extract_whole_nbr_positive(char **ptr, double *whole_nbr)
{




}

static inline int	extract_fractional_part(char **ptr, double *fraction)
{


}
