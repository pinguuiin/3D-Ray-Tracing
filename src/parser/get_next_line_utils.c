/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 11:32:19 by ykadosh           #+#    #+#             */
/*   Updated: 2025/01/15 15:59:59 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	process_buffer(char **line, char *buffer, size_t *i)
{
	size_t	j;

	while (buffer[*i])
	{
		if (buffer[*i] == '\n')
		{
			(*i) += 1;
			*line = update_line(*line, buffer, i);
			if (!*line)
				return (-1);
			j = *i;
			while (buffer[j])
				j++;
			if (j == *i)
				buffer[0] = '\0';
			else
			{
				buffer = ft_memmove_mod(buffer, &buffer[*i], j - (*i));
				buffer[j - (*i)] = '\0';
			}
			return (1);
		}
		(*i)++;
	}
	return (0);
}

char	*update_line(char *line, char *buffer, size_t *i)
{
	char	*beginning;
	char	*ending;

	beginning = NULL;
	ending = NULL;
	if (line)
		beginning = line;
	ending = (char *)malloc(sizeof(char) * ((*i) + 1));
	if (!ending)
	{
		if (beginning)
			free(beginning);
		return (NULL);
	}
	ft_memmove_mod(ending, buffer, *i);
	ending[*i] = '\0';
	line = ft_strjoin_mod(beginning, ending);
	free(beginning);
	free(ending);
	return (line);
}

void	*ft_memmove_mod(void *dest, const void *src, size_t n)
{
	size_t	temp;

	if (!dest && !src)
		return (NULL);
	if (n == 0)
		return (dest);
	if (dest > src)
	{
		while (n > 0)
		{
			n--;
			((char *)dest)[n] = ((char *)src)[n];
		}
	}
	else
	{
		temp = n;
		while (n > 0)
		{
			*(char *)dest++ = *(char *)src++;
			n--;
		}
		dest = dest - temp;
	}
	return (dest);
}

char	*ft_strjoin_mod(const char *s1, const char *s2)
{
	char	*result;
	size_t	s1_len;
	size_t	s2_len;

	result = NULL;
	s1_len = 0;
	s2_len = 0;
	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup_mod(s2));
	if (!s2)
		return (ft_strdup_mod(s1));
	while (s1[s1_len])
		s1_len++;
	while (s2[s2_len])
		s2_len++;
	result = (char *)malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (!result)
		return (NULL);
	result = (char *) ft_memmove_mod(result, s1, s1_len);
	result += s1_len;
	result = (char *) ft_memmove_mod(result, s2, s2_len + 1);
	result -= s1_len;
	return (result);
}

char	*ft_strdup_mod(const char *s)
{
	char	*duplicate;
	size_t	length;

	duplicate = NULL;
	length = 0;
	if (!s)
		return (NULL);
	while (s[length])
		length++;
	duplicate = (char *)malloc(sizeof(char) * (length + 1));
	if (!duplicate)
		return (NULL);
	length = 0;
	while (s[length])
	{
		duplicate[length] = s[length];
		length++;
	}
	duplicate[length] = '\0';
	return (duplicate);
}
