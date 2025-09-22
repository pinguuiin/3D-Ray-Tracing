/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 21:47:12 by ykadosh           #+#    #+#             */
/*   Updated: 2025/09/21 21:47:53 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// WARN: get_next_line() does not let you to differentiate between failures:
// malloc() failure, read() failure, negative fd, if BUFFER_SIZE is defined to
// 0 or is unreasonably large, or EOF has beeen reached ---> all those return
// NULL from get_next_line().
// I find that rather ridiculous. I'd like to pass another pointer to get_next_line(),
// which would serve as a flag for the failures that should trigger a cleanup
// and exit.... But then, get_next_line() would not fit the prototype that was
// required by the original subject - is that okay?
// TODO: make a set_exit_code() function like in minishell?

void	parse_scene(t_info *info, char *file_name)
{
	int		fd;
	char	*line;
	// int		flag;

	line = NULL;
	// flag = 0;

// FIXME: should we accept an input argument which contains the filename but
// also whitespace / extra characters further down the line?

	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		// TODO: free_all_and_exit();
	}

	// call get_next_line() in a while loop, and parse all elements of the scene!
	// WARN: remember to check whether the structs that you are using here are
	// already allocated for previously!!!
	// NOTE: remember to free() the returned line before each call to
	// get_next_line() and at the very end, after the loop.

	while (!get_next_line_ultimate(fd, line))
	{
		if (!line)
			return ;
		// parse 'line', now that it is ready for parsing.
	}
	if (line)
		free(line); // might be unnecessary, have to check it
	// TODO: free_all_and_exit(); // you can exit with the code of set_exit_code().




	// TODO: consider allowing comments in the .rt scene text file:
	// All commented lines could, for example, start with a '#' sign - and
	// those lines would be ignored. Otherwise, the .rt file is so complicated
	// to make sense of, but comments there would be of great help! :-)


}
