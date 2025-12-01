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

# include "minirt.h"
# include "get_next_line_revised.h"

# include <fcntl.h>	// open(), close()

typedef enum e_status
{
	NO_ERROR			=	0,
	INVALID_INPUT		= 	-1,
	ALLOCATION_FAILURE	=	-2,
	OPEN_FAILURE		=	-3,
	CLOSE_FAILURE		=	-4,
	READ_FAILURE		=	-5,
	BUFFER_SIZE_ERROR	=	-6,
	LOAD_TEXTURE_FAIL	=	-7

}	t_status;

typedef enum e_list_id
{
	LIGHT,
	OBJECT
}	t_list_id;

# ifndef BONUS
# else
/* wrapper for each t_light node, only needed in parsing */
typedef struct s_node_light
{
	t_light				light; // current 'light' element
	struct s_node_light	*next;
}	t_node_light;
# endif

typedef struct s_node_obj
{
	t_object			object;
	struct s_node_obj	*next;

}	t_node_obj;

# ifndef BONUS
typedef struct s_parser
{
	int			fd;
	size_t		line_num;
	int			n_lights;
	int			n_spheres;
	int			n_planes;
	int			n_cylinders;
	int			n_ambs;
	int			n_cams;

	// objects linked list
	// 'current' allows creation of nodes to happen faster, without walking
	// through the list on each iteration, since it always points at the 'hole'
	// for the node to be created (except at the very first iteration, but in
	// that case 'head' is already pointing there).
	t_node_obj	*head;
	t_node_obj	*current;

}	t_parser;
# else
typedef struct s_parser
{
	int				fd;
	size_t			line_num;
	int				n_lights;
	int				n_spheres;
	int				n_planes;
	int				n_cylinders;
	int				n_ambs;
	int				n_cams;

	// objects linked list
	// 'current' allows creation of nodes to happen faster, without walking
	// through the list on each iteration, since it always points at the 'hole'
	// for the node to be created (except at the very first iteration, but in
	// that case 'head' is already pointing there).
	t_node_obj		*head;
	t_node_obj		*current;

	// light linked list
	t_node_light	*head_light;
	t_node_light	*curr_light;

}	t_parser;
# endif

// scene and elements parsing
void	parse_argument(int argc, char *argv[]);
void	parse_scene(t_info *info, char *filename);
int		parse_ambient_lighting(t_color *amb, char *str, t_parser *parser);
int		parse_camera(t_cam *cam, char *str, t_parser *parser);
int		parse_sphere(t_parser *parser, char *str, size_t line_num);
int		parse_plane(t_parser *parser, char *str, size_t line_num);
int		parse_cylinder(t_parser *parser, char *str, size_t line_num);
# ifndef BONUS
int		parse_light(t_parser *parser, char *str, t_light *light);
# else
int		parse_light(t_parser *parser, char *str);
int		parse_texture_for_sphere(char **str, t_object *sphere, size_t line_num);
# endif

// parsing utilities
bool	ft_isspace(int c);
bool	isspace_but_not_newline(int c);
void	skip_whitespace(char **str);
void	skip_whitespace_but_not_newline(char **str);
int		ft_strtod(char **str, double *result, size_t line_num);
int		parse_color(char **str, t_color *color, double *ratio, size_t line_num);
void	apply_ratio_to_color(t_color *color, double ratio, bool is_provided);
int		parse_3d_vector(char **str, t_vec *vector, size_t line_num);
bool	is_valid_separator(char	**str, size_t line_num);
bool	is_valid_tail_when_expecting_more_data(char **str, size_t line_num);
bool	is_valid_end_of_line(char *s, size_t line_num);
bool	is_valid_n_elements(t_parser *parser, t_list_id id);
bool	is_within_range_vector(t_vec *vec, size_t line_num);

// error messaging and memory management
int		handle_parsing_error(t_status status, char *line, t_parser *parser);
void	display_parsing_error(char *msg, size_t line_num);
int		clean_up_parser(t_parser *parser, char *line);

#endif
