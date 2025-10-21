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

#include "get_next_line.h"

static char	*review_line_and_clean_buffer(char *line, char *buffer, size_t *i);
static int	read_and_manage_output(int fd, char *buffer, char **line);

/*
 NOTE: regarding the variable "line_status":
	line_status =  1 : line is valid and should be returned to main
	line_status = -1 : malloc() for line has failed; NULL should be
		returned all the way up to main, in order to communicate the failure.
	line_status =  0 : line is still valid (or NULL, but valid); it should
		NOT be returned to main yet
	line_status = -2 : signals failure of read() within the helper function
		read_and_manage_output().
*/

// Return values:
//  0: success; next line is ready
// -1: malloc() failure
// -2: read() failure
// -3: BUFFER_SIZE is 0 (defaults to 1024 in get_next_line's header)
int	get_next_line_minirt(int fd, char **output)
{
	static char	buffer[BUFFER_SIZE + 1];
	int			line_status;
	size_t		i;
	char		*line;

	line = NULL;
	if (!BUFFER_SIZE)
		return (-3);
	line_status = 0;
	i = 0;
	if (buffer[i])
	{
		line_status = process_buffer(&line, buffer, &i);
		if (line_status == -1) // no worries here: line is always NULL before the line above this one, no need to free it.
			return (-1);
		if (line_status == 1) // next line is ready: OK to return
		{
			*output = line;
			return (0);
		}
		line = review_line_and_clean_buffer(line, buffer, &i);
		if (!line)
			return (2);
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
		if (line_status == -1 || line_status == 1)
			return (line_status);
		*line = review_line_and_clean_buffer(*line, buffer, &i);
		if (!*line)
			return (-1);
		if (bytes_read < BUFFER_SIZE)
			return (1);
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	if (bytes_read == -1)
		return (-2);
	return (1);
}
