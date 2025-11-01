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
// # include <fcntl.h>	// open(), close()
// # include <stdint.h>	// fixed-width data types (such as uint32_t, uint64_t)

# include "minirt.h"
# include "get_next_line_revised.h"

typedef enum e_status
{
	NO_ERROR			=	0,
	INVALID_INPUT		= 	-1,
	ALLOCATION_FAILURE	=	-2,
	OPEN_FAILURE		=	-3,
	CLOSE_FAILURE		=	-4,
	READ_FAILURE		=	-5,
	BUFFER_SIZE_ERROR	=	-6

}	t_status;

enum e_exit_code
{
	SUCCESS			=	0,
	MLX_FAILURE		=	1,
	INPUT_ERROR		=	2,
	SYSTEM_FAILURE	=	3

};

// FIXME: delete this when sure not to use it (only used in order to create_new_node()
// using the very same function for both linked lists (LIGHT and OBJECT),
// but I am probably going to abandon that idea, the function is soooo complicated
// and odd looking. Better revert back too the original split functions, create_new_light_node()
// and creat_new_obj_node()
typedef enum e_list_id
{
	LIGHT,
	OBJECT
}	t_list_id;

// wrapper for each t_light node, only needed in parsing
typedef struct s_node_light
{
	t_light				light; // current 'light' element
	struct s_node_light	*next;
	// FIXME: when compiling the project, try to see if replacing 'struct s_node_light'
	// with 't_node_light' would work. I suppose it wouldn't compile since it is
	// not yet declared in this scope!

}	t_node_light;

typedef struct s_node_obj
{
	t_object			object;
	struct s_node_obj	*next;

}	t_node_obj;

typedef struct s_parser
{
	int				fd;

	// light linked list
	t_node_light	*head;
	t_node_light	*current;

	// objects linked list
	// 'curr_obj' allows creation of nodes to happen faster, without walking
	// through the list on each iteration, since it always points at the 'hole'
	// for the node to be created (except at the very first iteration, but in
	// that case 'head' is already pointing there).
	t_node_obj		*head_obj;
	t_node_obj		*curr_obj;

	uint32_t		line_num;
	uint32_t		n_lights;
	uint32_t		n_spheres;
	uint32_t		n_planes;
	uint32_t		n_cylinders;
	uint8_t			n_ambs;
	uint8_t			n_cams;

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

}	t_color;

typedef t_vec	t_vec;

// scene and elements parsing
void	parse_argument(int argc, char *argv[]);
void	parse_scene(t_info *info, char *filename);
int		parse_ambient_lighting(t_color *amb, char *str, t_parser *parser);
int		parse_camera(t_cam *cam, char *str, t_parser *parser);
int		parse_light(t_parser *parser, char *str);
int		parse_sphere(t_parser *parser, char *str, uint32_t line_num);
int		parse_plane(t_parser *parser, char *str, uint32_t line_num);
int		parse_cylinder(t_parser *parser, char *str, uint32_t line_num);

// parsing utilities
bool	ft_isspace(int c);
bool	isspace_but_not_newline(int c);
void	skip_whitespace(char **str);
void	skip_whitespace_but_not_newline(char **str);
int		ft_strtod(char **str, double *result, uint32_t line_num);
int		parse_color(char **str, t_color *color, double *ratio, uint32_t line_num);
void	apply_ratio_to_color(t_color *color, double ratio, bool is_provided);
int		parse_3d_vector(char **str, t_vec *vector, uint32_t line_num);
bool	is_valid_separator(char	**str, uint32_t line_num);
bool	is_valid_tail_when_expecting_more_data(char **str, uint32_t line_num);
bool	is_valid_end_of_line(char *s);
int		create_new_node(void *head, void *current, t_list_id id, size_t size);
bool	is_within_range_vector(t_vec *vec, uint32_t line_num);

// error messaging and memory management
int		handle_parsing_error(t_status status, char *line, t_parser *parser);
void	display_parsing_error(const char *msg, uint32_t line_num);
int		clean_up_parser(t_parser *parser, char *line);

#endif
