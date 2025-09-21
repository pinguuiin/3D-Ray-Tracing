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

void	parse_scene(t_info *info, char *file_name)
{
	static char	buffer[1024];	// WARN: in my get_next_line(), amount allocated is BUFFER_SIZE + 1... should I do the same?
	int			fd;

	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		// TODO: free_all_and_exit();
	}

	// call get_next_line() in a while loop, and parse all elements of the scene!



	// TODO: consider allowing comments in the .rt scene text file:
	// All commented lines could, for example, start with a '#' sign - and
	// those lines would be ignored. Otherwise, the .rt file is so complicated
	// to make sense of, but comments there would be of great help! :-)



}
