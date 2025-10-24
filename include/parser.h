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

// FIXME: lists of lights and of objects.
// Should be all 'transferred' once parsing is complete and validated:
// 1. all data copied to the array of lights 'light' (which should become a pointer)
// and the array of objects 'obj' in 'info' struct.
// 2. These lists should be freed and cleaned up once 'transferred'
// 3. If this structure is successful and we go through with it:
// 	consider deleting the obj_lst member of 'info', and you would only have a
// 	pointer to this parsing instead.
// FIXME: ideally, you don't even want to have a pointer nor this struct itself
// within the 'info' struct. You want to declare it in in one of parsing's main
// stack frames, and pass it around by reference;
// Once you come back from that main parsing stack frame, we should not have
// use for it at all -> we would have already copied its contains to the arrays
// which are in 'info' and which will be used by the renderer.

// wrapper for each t_light node, only needed in parsing
typedef struct s_node_light
{
	t_light				current; // current 'light' element
	struct s_node_light	*next;
	// FIXME: when compiling the project, try to see if replacing 'struct s_node_light'
	// with 't_node_light' would work. I suppose it wouldn't compile since it is
	// not yet declared in this scope!

}	t_node_light;

typedef struct s_parser
{
	t_node_light	*head; // head of list
	t_node_light	**tail; // might be unnecessary - or might allow not needing to go through the whole list every time you need to allocate a new node?

	t_object		*obj_list;
	// t_node_obj	*curr_obj;

	uint8_t			n_ambs;
	uint8_t			n_cams;
	uint32_t		n_light;
	uint32_t		n_objs;

}	t_parser;

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
void	parse_scene(char *file_name);
int		parse_ambient_lighting(t_color *amb, char *str, uint32_t line_num,
			uint8_t *n_ambs);
int		parse_camera(t_cam *cam, char *str, uint8_t line_num, uint8_t *n_cams);
int		parse_light(t_light *light, char *str, uint32_t line_num);
int		parse_sphere(char *str, uint32_t line_num);
int		parse_plane(char *str, uint32_t line_num);
int		parse_cylinder(char *str, uint32_t line_num);

// parsing utilities
int		ft_isspace(int c);
int		isspace_but_not_newline(int c);
void	skip_whitespace(char **str);
void	skip_whitespace_but_not_newline(char **str);
int		ft_strtod(char **str, double *result, uint32_t line_num);
int		parse_color(char **str, t_color *color, double ratio);
void	apply_ratio_to_color(t_color *color, double ratio, bool is_provided);
int		parse_coordinates(char **str, t_vec *position, uint32_t line_num);
int		parse_light(t_light *light, char *str, uint32_t line_num);
int		parse_direction_vector(char **str, t_vec *direction, uint32_t line_num);
bool	is_valid_separator(char	**str);
bool	is_valid_tail_when_expecting_more_data(char **str, uint32_t line_num);
bool	is_valid_end_of_line(char *s);

// error handling and memory management
int		handle_parsing_error(int error_code, char *line, t_parser *parser);
void	display_parsing_error(const char *msg, uint32_t line_num);
void	clean_up_parsing_memory(t_parser *parser, char *line);

#endif
