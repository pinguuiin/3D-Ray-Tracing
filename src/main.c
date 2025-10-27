/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 15:59:24 by ykadosh           #+#    #+#             */
/*   Updated: 2025/10/01 19:01:12 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int		parse_input_argument(char *arg);
static size_t	skip_non_whitespace_chars_until_file_extension(char **ptr);
static void		handle_unexpected_arg_count(int argc);

// return values:
//	1:	MLX function failures (WARN: not sure about this one!)
//	2:	invalid input from .rt file
//	3:	fatal system error during parsing (open(), malloc(), read(), empty
//	buffer for get_next_line_minirt())
// 	0:	If program ran smoothly
int	main(int argc, char *argv[])
{
	t_info	*info;

	// info is a static variable within get_info(), so its values are zeroed.
	info = get_info();

	if (argc == 2)
	{
		parse_input_argument(argv[1]);
		parse_scene(info, argv[1]);
	}
	else
	{
		handle_unexpected_arg_count(argc);
		return (1);
	}

	// graphic rendering
	initialize_mlx(&info);
	mlx_key_hook(info.mlx, &key_handler, &info);
	mlx_loop_hook(info.mlx, draw, &info);
	mlx_loop(info.mlx);

	free_exit(NULL); // this does not actually exit the program, no worries
	return (0);
}

static void	handle_unexpected_arg_count(int argc)
{
	if (argc == 1)
	{
		write(2, "No input scene file provided.\nPlease provide valid "
			".rt file for the miniRT program to render - for example:\n"
			"./miniRT beautiful_scene.rt\n",
			sizeof ("No input scene file provided.\nPlease provide valid "
			".rt file for the miniRT program to render - for example:\n"
			"./miniRT beautiful_scene.rt\n") - 1);
	}
	else
	{
		write(2, "Too many arguments provided. Only one valid .rt scene "
			"file is accepted.\n",
			sizeof ("Too many arguments provided. Only one valid .rt scene "
			"file is accepted.\n") - 1);
	}
}

// TESTING: required!
// FIXME: This would not accept relative paths!!!! because of the '.' limitation.
// Try to make it accept:
// ./ and
// ../
static int	parse_input_argument(char *arg)
{
	char	*ptr;
	char	*temp;
	bool	should_copy;
	size_t	len;

	ptr = arg;
	should_copy = 0;
	len = 0;
	skip_whitespace(&ptr);
	if (ptr != arg)
		should_copy = 1;
		

	// WARN: careful: we might be on the null terminator at this point.
	len = skip_non_whitespace_chars_until_file_extension(&ptr);

	if (*ptr == '.')
		ptr++;
	if (*ptr == 'r' && *(ptr + 1) == 't'
		&& (!*(ptr + 2) || ft_isspace(*(ptr + 2))))
	{
		ptr += 2;
		len += 4; // we add to len: '.rt\0'
		temp = ptr;
		skip_whitespace(&temp);
		if (*temp)
		{
			ft_putstr_fd("Invalid argument provided. Only a single valid "
				".rt file is accepted.\n", 2);
			return (-1);
		}
		if (temp != ptr)
			should_copy = 1;

		// WARN: does this next line even work as I want it to? would the string
		// passed to parse_scene(), which would simply be argv[1], stop at the first null
		// terminator it sees?
		*ptr = '\0';
		ptr -= (len - 1);

		if (should_copy)
		{
			arg = ft_memmove(arg, ptr, len);

		}
	}
	else
	{
		ft_putstr_fd("Unsupported file type detected; "
			"please provide a valid .rt file.\n", 2);
		return (-1);
	}
	return (0);
}

static size_t	skip_non_whitespace_chars_until_file_extension(char **ptr)
{
	char	*s;
	size_t	i;

	i = 0;
	s = *ptr;
	while (*s && *s != '.' && *s != ' ' && (*s < '\t' || *s > '\r'))
	{
		i++;
		s++;
	}
	*ptr = s;
	return (i);
}
