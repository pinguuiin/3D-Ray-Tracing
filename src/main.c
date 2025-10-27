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
static size_t	skip_non_whitespace_chars_until_file_extension(char **temp_1);
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
		// FIXME: write a function that parses through the file's name, making
		// sure that it is a filename with no strange tails, that it ends
		// with ".rt" (!!!) and nothing else behind, except, if you want to be
		// very forgiving : accept whitespace, if the user would provide the
		// argument within quotes........
		// parse_scene() opens the whole argv[1] - consider cleaning up that
		// file name into a different string, if necessary, and pass that string
		// instead, allowing it to be freed from there as soon as the file is closed
		// AND every time you call free_exit() or clean_up_parsing_and_exit().
		// NOTE: Alternatively: can you simply overwrite the beginning of argv[1],
		// directly, null terminating it yourself, thus avoiding all trouble with
		// malloc() and free()???
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
static int	parse_input_argument(char *arg)
{
	char	*temp_1;
	char	*temp_2;
	bool	should_copy;
	size_t	len;

	temp_1 = arg;
	should_copy = 0;
	len = 0;
	skip_whitespace(&temp_1);
	if (temp_1 != arg)
		should_copy = 1;
		

	// WARN: careful: we might be on the null terminator at this point.
	len = skip_non_whitespace_chars_until_file_extension(&temp_1);

	if (*temp_1 == '.')
		temp_1++;
	if (*temp_1 == 'r' && *(temp_1 + 1) == 't'
		&& (!*(temp_1 + 2) || ft_isspace(*(temp_1 + 2))))
	{
		temp_1 += 2;
		len += 4; // we add to len: '.rt\0'
		temp_2 = temp_1;
		skip_whitespace(&temp_2);
		if (*temp_2)
		{
			ft_putstr_fd("Invalid argument provided. Only a single valid "
				".rt file is accepted\n", 2);
			return (-1);
		}
		if (temp_2 != temp_1)
			should_copy = 1;

		// WARN: does this next line even work as I want it to? would the string
		// passed to parse_scene(), which would simply be argv[1], stop at the first null
		// terminator it sees?
		*temp_1 = '\0';
		temp_1 -= (len - 1);

		if (should_copy)
		{
			arg = ft_memmove(arg, temp_1, len);

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

static size_t	skip_non_whitespace_chars_until_file_extension(char **temp_1)
{
	char	*s;
	size_t	i;

	i = 0;
	s = *temp_1;
	while (*s && *s != '.' && *s != ' ' && (*s < '\t' || *s > '\r'))
	{
		i++;
		s++;
	}
	*temp_1 = s;
	return (i);
}
