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

static void	handle_gnl_error(t_info *info, int gnl_flag);
static int	parse_line(t_info *info, char *line);

void	parse_scene(t_info *info, char *file_name)
{
	int		fd;
	char	*line;
	int		line_num;
	int		gnl_flag;

	line = NULL;
	line_num = 1;
	gnl_flag = 0;

// FIXME: should we accept an input argument which contains the filename but
// also whitespace / extra characters further down the line?

// FIXME: SET STRINGS MANUALLY, FINALLY, don't use strerror()!!!
// and control the exit values using gnl_flag.
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		free_exit(info, "File opening failed");
	while (!gnl_flag)
	{
		gnl_flag = minirt_get_next_line(fd, &line);
		if (line)
		{
			parse_line(info, line);
			free(line);
			line = NULL;
			line_num++;
		}
	}
	if (gnl_flag)
		handle_gnl_error(info, gnl_flag);
		// free_exit(info, strerror(errno)); // deprecated


	// TODO: consider allowing comments in the .rt scene text file:
	// All commented lines could, for example, start with a '#' sign - and
	// those lines would be ignored. Otherwise, the .rt file is so complicated
	// to make sense of, but comments there would be of great help! :-)

	// FIXME:
	// once you transfer the objects list/s into into the array of all of the
	// objects' structs, organize them in order ----> the count for each will
	// allow us to optimize.
}


static void	handle_gnl_error(t_info *info, int gnl_flag)
{
	if (gnl_flag == 1)
		// free_exit(info, "something "); // TODO: 
	if (gnl_flag == 2)
		// free_exit(info, "something "); // TODO: 
	if (gnl_flag == 3)
		// free_exit(info, "something "); // TODO: 
	if (gnl_flag == 4)
		// free_exit(info, "something "); // TODO: 
	if (gnl_flag == 5)
		// free_exit(info, "something "); // TODO: 

}

// NOTE: free the line from here when an invalid input is found!
// NOTE: Do I need a double pointer? probably not, as I free only if there is an
// invalid input and then we exit the program anyways.
static int	parse_line(t_info *info, char *line)
{







}
