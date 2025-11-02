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

#include "parser.h"

static void		handle_unexpected_arg_count(int argc);
static bool		is_valid_file_extension(char *arg);

void	parse_argument(int argc, char *argv[])
{
	if (argc != 2)
	{
		handle_unexpected_arg_count(argc);
		exit (2);
	}
	if (!is_valid_file_extension(argv[1]))
	{
		ft_putstr_fd("Unsupported file type detected; "
			"please provide a valid .rt file.\n", 2);
		exit (2);
	}
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

static bool	is_valid_file_extension(char *arg)
{
	size_t	i;

	i = 0;
	while (arg[i])
		i++;
	if (i >= 3)
	{
		arg = &arg[i - 3];
		if (*arg == '.' && *(arg + 1) == 'r' && *(arg + 2) == 't')
			return (1);
	}
	return (0);
}
