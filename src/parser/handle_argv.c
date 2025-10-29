/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_argv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 21:14:53 by ykadosh           #+#    #+#             */
/*   Updated: 2025/10/27 21:15:01 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void		handle_unexpected_arg_count(int argc);
static size_t	skip_filename_and_count_length(char **ptr);
static bool		is_valid_file_extension(char *s, size_t len);

int	parse_argument(int argc, char *argv[])
{
	char	*ptr;
	char	*temp;
	bool	should_copy;
	size_t	len;

	if (argc != 2)
	{
		handle_unexpected_arg_count(argc);
		return (-1);
	}


	ptr = argv[1];
	should_copy = 0;
	len = 0;
	skip_whitespace(&ptr);
	if (ptr != argv[1])
		should_copy = 1;

	len = skip_filename_and_count_length(&ptr);

	// validate ".rt" end of file name (only valid extension for input file)
	if (!is_valid_file_extension(ptr, len))
	{
		ft_putstr_fd("Unsupported file type detected; "
			"please provide a valid .rt file.\n", 2);
		return (-1);
	}

	// scan the rest of the string, and accept only whitespace until null terminator.
	temp = ptr;
	skip_whitespace(&temp);
	if (*temp)
	{
		ft_putstr_fd("Invalid argument provided. only a single valid "
			".rt file is accepted.\n", 2);
		return (-1);
	}
	if (should_copy)
		argv[1] = ft_memmove(argv[1], ptr - len, len);
	argv[1][len] = '\0';
	return (0);
}

static size_t	skip_filename_and_count_length(char **ptr)
{
	char	*s;
	size_t	len;

	len = 0;
	s = *ptr;

	while (*s && *s != ' ' && (*s < '\t' || *s > '\r'))
	{
		s++;
		len++;
	}
	*ptr = s;
	return (len);
}

static bool	is_valid_file_extension(char *s, size_t len)
{
	if (len >= 3)
	{
		s -= 3;
		if (*s++ == '.' && *s++ == 'r' && *s++ == 't')
			return (1);
	}
	return (0);
}

static void	handle_unexpected_arg_count(int argc)
{
	if (argc == 1)
	{
		ft_putstr_fd("No input scene file provided.\nPlease provide a valid "
			".rt file for the miniRT program to render - for example:\n"
			"./miniRT beautiful_scene.rt\n", 2);
	}
	else
	{
		ft_putstr_fd("Too many arguments provided. Only one valid .rt scene "
			"file is accepted.\n", 2);
	}
}
