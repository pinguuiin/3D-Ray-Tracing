/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 21:19:02 by ykadosh           #+#    #+#             */
/*   Updated: 2025/01/15 13:40:53 by ykadosh          ###   ########.fr       */
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

// FIXME: add "set_exit_code()" calls in all relevant spots.
// TODO: observe output - line
int	get_next_line_ultimate(int fd, char **output)
{
	static char	buffer[BUFFER_SIZE + 1];
	int			line_status;
	size_t		i;
	char		*line;


	line = NULL;
	if (fd < 0 || !BUFFER_SIZE)
		return (NULL);
	line_status = 0;
	i = 0;
	if (buffer[i])
	{
		line_status = process_buffer(&line, buffer, &i);
		if (line_status == 1 || line_status == -1)
			return (line);
		line = review_line_and_clean_buffer(line, buffer, &i);
		if (!line)
			return (NULL);
	}
	if (read_and_manage_output(fd, buffer, &line) < 0)
	{
		free(line);
		line = NULL;
	}
	*output = line;
	return (line);
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
