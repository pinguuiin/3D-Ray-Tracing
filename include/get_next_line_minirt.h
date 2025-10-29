/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_minirt.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 11:32:35 by ykadosh           #+#    #+#             */
/*   Updated: 2025/09/30 18:28:48 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_MINIRT_H
# define GET_NEXT_LINE_MINIRT_H

/*
* NOTE: We define buffer size if it hasn't been defined when compiling.
* Size 1024 is a well-established convention providing good balance between
* performance, memory efficiency and portability with modern hardware; That's
* why it has been chosen as the default size. But we can compile the function
* with a different BUFFER_SIZE, using flag -D BUFFER_SIZE=<n>.
*/

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

/*
* NOTE: get_next_line() uses system call read() with a given file descriptor
* and returns the next line from that file, in order of appearance: The first
* call returns the first line, the second returns the second... If it is called
* sufficient times until there is no more data in the file, all of the file's
* contains are returned.
* RETURN VALUES:	1. A pointer to the first character of the line, or
* 					2. NULL - in the following cases:
* 	a.	if EOF has been reached and there are no more lines to return (or if
* 		the file is empty)
* 	b.	upon failure of read()
* 	c.	upon memory allocation failure
* 	d.	if BUFFER_SIZE is defined to 0
* 	e.	if BUFFER_SIZE is unreasonably large
* 	f.	if the file descriptor is a negative one
*
* WARN: The returned line is dynamically allocated within get_next_line();
* Therefore, it is the caller's responsibility to remember freeing that memory,
* if they want to avoid memory leaks.
*/

int		get_next_line_minirt(int fd, char **output);
int		process_buffer(char **line, char *buffer, size_t *i);
char	*update_line(char *line, char *buffer, size_t *i);
void	*ft_memmove_mod(void *dest, const void *src, size_t n);
char	*ft_strjoin_mod(const char *s1, const char *s2);
char	*ft_strdup_mod(const char *s);

#endif
