/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_minirt.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 21:19:02 by ykadosh           #+#    #+#             */
/*   Updated: 2025/09/30 18:28:49 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static char	*review_line_and_clean_buffer(char *line, char *buffer, size_t *i);
static int	read_and_manage_output(int fd, char *buffer, char **line);

/*
 NOTE: regarding the variable "line_status":
	line_status =  1 : line is valid and should be returned to main
	line_status =  0 : line is still valid (or NULL, but valid); it should
		NOT be returned to main yet
	line_status = ALLOCATION_FAILURE: malloc() for line has failed, and that
		should be communicated to the caller.
	line_status = READ_FAILURE : signals failure of read() within the helper
		function read_and_manage_output().
*/

// FIXME: cap the BUFFER_SIZE to something reasonable?
// So we don't have some funny surprises. And handle that error with returning BUFFER_SIZE_ERROR,
// just as we do for the empty buffer ----> but this would require review of the notes
// regarding buffer_size_error or handle_parsing_error() etc.

/*
* Return values:
*	0: success; next line is ready
*	ALLOCATION_FAILURE: malloc() failure
*	READ_FAILURE: read() failure
*	BUFFER_SIZE_ERROR: BUFFER_SIZE has been predefined in compilation to zero
*/
int	get_next_line_minirt(int fd, char **output)
{
	static char	buffer[BUFFER_SIZE + 1];
	int			line_status;
	size_t		i;
	char		*line;

	line = NULL;
	if (!BUFFER_SIZE)
		return (BUFFER_SIZE_ERROR);
	line_status = 0;
	i = 0;
	if (buffer[i])
	{
		line_status = process_buffer(&line, buffer, &i);
		if (line_status == ALLOCATION_FAILURE) // no worries here: line is always NULL before the line above this one, no need to free it.
			return (ALLOCATION_FAILURE);
		if (line_status == 1) // next line is ready: OK to return
		{
			*output = line;
			return (0);
		}
		line = review_line_and_clean_buffer(line, buffer, &i);
		if (!line)
			return (ALLOCATION_FAILURE);
	}
	line_status = read_and_manage_output(fd, buffer, &line)
	if (line_status < 0)
	{
		free(line);
		line = NULL;
		return (line_status);
	}
	*output = line;
	return (0);
}

static char	*review_line_and_clean_buffer(char *line, char *buffer, size_t *i)
{
	line = update_line(line, buffer, i);
	if (!line)
		return (NULL);
	buffer[0] = '\0';
	return (line);
}

static int	read_and_manage_output(int fd, char *buffer, char **line)
{
	ssize_t	bytes_read;
	int		line_status;
	size_t	i;

	line_status = 0;
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		i = 0;
		line_status = process_buffer(line, buffer, &i);
		if (line_status == ALLOCATION_FAILURE || line_status == 1)
			return (line_status);
		*line = review_line_and_clean_buffer(*line, buffer, &i);
		if (!*line)
			return (ALLOCATION_FAILURE);
		if (bytes_read < BUFFER_SIZE)
			return (1);
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	if (bytes_read == -1)
		return (READ_FAILURE);
	return (1);
}
