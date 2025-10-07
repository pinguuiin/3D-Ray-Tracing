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

static inline int	extract_whole_nbr(char **s, double *whole_nbr);
static inline int	extract_fractional_part(char **s, double *fraction);

// TODO:
// WARN: consider error handling
inline int	ft_strtod(char **s, double *result)
{
	char	*ptr;	// for readability.
	int		is_neg;

	ptr = *s;
	is_neg = 0;
	// NOTE: is 'result' already set to zero when this function is called?
	// Should I set it to zero here, if not?
	if (extract_whole_nbr() == -1)
	{
		// TODO: handle the error
		// return
	}


	// TODO: parse the floating point '.'

	if (extract_fractional_part(s, result) == -1)


	// WARN: use math.h's 'isinf()' function, and perhaps also 'isnan()', to
	// check for the overflow of your double, before returning.
	// How should you handle the error, however, if it did overflow?

	if (isnan(*result) || isinf(*result))
	{
		// TODO: handle the error

	}

	*s = ptr;	// increments the pointer at the caller.
}

static inline int	extract_whole_nbr(char **s, double *whole_nbr)
{




}

static inline int	extract_fractional_part(char **s, double *fraction)
{


}
