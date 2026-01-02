/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_revised.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 21:19:02 by ykadosh           #+#    #+#             */
/*   Updated: 2025/10/30 18:06:39 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	parse_buffer(t_gnl *gnl, size_t *i, char **output, char *buffer);
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

/*
* Return values:
*	0: success; next line is either ready or empty (in case EOF is reached).
*	ALLOCATION_FAILURE: malloc() failure
*	READ_FAILURE: read() failure
*	BUFFER_SIZE_ERROR: BUFFER_SIZE has been predefined in compilation to zero
*/
int	get_next_line_revised(int fd, char **output)
{
	static char	buffer[BUFFER_SIZE + 1];
	t_gnl		gnl;
	size_t		i;

	gnl.line = NULL;
	gnl.line_status = 0;
	i = 0;
	if (BUFFER_SIZE <= 0)
		return (BUFFER_SIZE_ERROR);
	if (buffer[i])
	{
		if (parse_buffer(&gnl, &i, output, buffer) == ALLOCATION_FAILURE)
			return (ALLOCATION_FAILURE);
		if (gnl.line_status == 1)
			return (0);
	}
	gnl.line_status = read_and_manage_output(fd, buffer, &gnl.line);
	if (gnl.line_status < 0)
	{
		free(gnl.line);
		gnl.line = NULL;
		return (gnl.line_status);
	}
	*output = gnl.line;
	return (0);
}

static int	parse_buffer(t_gnl *gnl, size_t *i, char **output, char *buffer)
{
	gnl->line_status = process_buffer(&gnl->line, buffer, i);
	if (gnl->line_status == ALLOCATION_FAILURE)
		return (ALLOCATION_FAILURE);
	if (gnl->line_status == 1)
	{
		*output = gnl->line;
		return (0);
	}
	gnl->line = review_line_and_clean_buffer(gnl->line, buffer, i);
	if (!gnl->line)
		return (ALLOCATION_FAILURE);
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
