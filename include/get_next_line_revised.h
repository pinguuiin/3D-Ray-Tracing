/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_revised.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 11:32:35 by ykadosh           #+#    #+#             */
/*   Updated: 2025/10/30 18:06:38 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_REVISED_H
# define GET_NEXT_LINE_REVISED_H

/*
* NOTE: We define buffer size if it hasn't been defined when compiling.
* Size 1024 is a well-established convention providing good balance between
* performance, memory efficiency and portability with modern hardware; This is
* why it has been chosen as the default size. But one can compile the function
* with a different BUFFER_SIZE, using flag -D BUFFER_SIZE=<n>.
*/

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

/*
* NOTE: get_next_line_revised() is a variation of the get_next_line() function,
* which is specifically designed for the miniRT program, but could be of great
* use in many other ones. Instead of returning a pointer to the dynamically
* allocated line (which is basically a copy of the line starting at the position
* of the provided file descriptor's offset) or a NULL pointer (when failure
* occurs or when the offset is located on the EOF character), this version
* produces those exact same results as a side effect via the double pointer
* 'output' - an extra argument to this revised function.
* If the caller is responsible enough to only call this function when 'output'
* is NULL, this allows get_next_line_revised() to be able to return an integer
* value, communicating a more precise message to the caller. The return values
* depend on miniRT's parser.h, which defines a set of enums ("t_status"), but
* those can of course be tailored differently by setting other macros in this
* very header or elsewhere - as long as the failure macros (BUFFER_SIZE_ERROR,
* READ_FAILURE and ALLOCATION_FAILURE) are expressed by different negative
* values.
*
* RETURN VALUES:
* 	- BUFFER_SIZE_ERROR: BUFFER_SIZE is defined before compilation to zero,
* 		making get_next_line_revised()'s task impossible. Caller's 'output'
* 		pointer remains unchanged.
*	- ALLOCATION_FAILURE: Memory allocation request for the built line of
*		get_next_line_revised() has failed. The caller's 'output' pointer is set
*		to NULL.
*	- READ_FAILURE: Call to read() within get_next_line_revised() has failed;
*		attempt to build a line is aborted, and the caller's 'output' pointer
*		is set to NULL.
*	- 0: Success. In this scenario, 'output' is either pointing to:
*		- valid heap memory containing a copy of the file descriptor's "next
*		line", or to
*		- NULL, if the entirety of the file contains have been already read.
*
* WARN: Much like it is when working with get_next_line(), the caller has to
* always be responsibe in regards to 'output' before and after calling this
* function, since, upon a successful call, it would point to heap memory,
* dynamically allocated within the call - or, in the case of the revised
* version, it could be modified to NULL, and any trace 'output' is pointing to
* before the call could be lost.
* For those safety reasons, 'output' should therefore both:
*	- be a NULL pointer BEFORE passing it to this function, and
*	- be freed AFTER returning from it.
*/

int		get_next_line_revised(int fd, char **output);
int		process_buffer(char **line, char *buffer, size_t *i);
char	*update_line(char *line, char *buffer, size_t *i);
void	*ft_memmove_mod(void *dest, const void *src, size_t n);
char	*ft_strjoin_mod(const char *s1, const char *s2);
char	*ft_strdup_mod(const char *s);

#endif
