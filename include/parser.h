/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:12:54 by ykadosh           #+#    #+#             */
/*   Updated: 2025/10/03 19:25:20 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

// WARN: only add these libraries here if they are not on minirt.h - or perhaps
// leave them still here as they are relevant to the parsing?
// # include <fcntl.h>		// open(), close()
# include <stdint.h>	// fixed-width data types (such as uint32_t, uint64_t)

/*
* 3d vector which can be used interchangeably for:
*	- x, y, z coordinates
*	- RGB color channel
*	- UVW mapping (for textures)
*/
typedef union u_vec
{
	struct
	{
		double	x;
		double	y;
		double	z;
	};

	struct
	{
		double	r;
		double	g;
		double	b;
	};

	struct
	{
		double	u;
		double	v;
		double	w;
	};

}	t_vec;

typedef t_vec	t_color;

// scene elements parsing
int		parse_ambient_lighting(t_ambient *amb, char *str, uint32_t line_num);
int		parse_camera(t_cam *cam, char *str, uint32_t line_num);
int		parse_light(t_light *light, char *str, uint32_t line_num);
int		parse_sphere(char *str, uint32_t line_num);
int		parse_plane(char *str, uint32_t line_num);
int		parse_cylinder(char *str, uint32_t line_num);

// parsing utilities
int		isspace_but_not_newline(int c);
int		ft_isspace(int c);
int		ft_strtod(char **str, double *result, uint32_t line_num);
int		parse_color(char **str, t_color *color);
int		parse_coordinates(char **str, t_vec *position, size_t line_num);
int		is_valid_separator(char	**str);

// error handling and memory management
void	handle_gnl_error_and_exit(t_info *info, int gnl_flag);
void	display_parsing_error(const char *msg, uint32_t line_num);
void	clean_up_parsing_memory(t_info *info, char *line);

#endif
