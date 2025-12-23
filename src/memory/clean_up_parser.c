/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 18:06:10 by ykadosh           #+#    #+#             */
/*   Updated: 2025/12/09 18:22:00 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	destroy_obj_list(t_parser *parser);

#ifndef BONUS
#else

static void	destroy_light_list(t_parser *parser);
#endif

/*
* This function frees the dynamically allocated memory used by parsing, i.e:
* - linked list for 'lights' (only when BONUS is defined)
* - linked list for all 'objects'
* - 'line', returned by get_next_line_revised()
* It also closes the file descriptor associated with the input .rt file.
*
* Return Values:
*	0:	Dynamically allocated memory was properly freed and file descriptor
*		was properly closed.
*	CLOSE_FAILURE:	Allocated memory was successfully freed, but close() has
*		failed. It is safe to end the program.
*/
#ifndef BONUS

int	clean_up_parser(t_parser *parser)
{
	destroy_obj_list(parser);
	if (parser->line)
		free(parser->line);
	if (close(parser->fd) == -1)
	{
		ft_putstr_fd("Failed to close input file. Aborting miniRT.\n", 2);
		return (CLOSE_FAILURE);
	}
	return (0);
}

static void	destroy_obj_list(t_parser *parser)
{
	t_node_obj		*current;
	t_node_obj		*next;

	current = parser->head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
}
#else

int	clean_up_parser(t_parser *parser)
{
	destroy_light_list(parser);
	destroy_obj_list(parser);
	if (parser->line)
		free(parser->line);
	if (close(parser->fd) == -1)
	{
		ft_putstr_fd("Failed to close input file. Aborting miniRT.\n", 2);
		return (CLOSE_FAILURE);
	}
	return (0);
}

static void	destroy_light_list(t_parser *parser)
{
	t_node_light	*current;
	t_node_light	*next;

	current = parser->head_light;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
}

static void	destroy_obj_list(t_parser *parser)
{
	t_node_obj		*current;
	t_node_obj		*next;

	current = parser->head;
	while (current)
	{
		next = current->next;
		if (!parser->is_valid_scene)
		{
			if (current->tex_file)
				free(current->tex_file);
			if (current->normal_file)
				free(current->normal_file);
			if (current->texture)
				mlx_delete_texture(current->texture);
			if (current->normal)
				mlx_delete_texture(current->normal);
		}
		free(current);
		current = next;
	}
}
#endif
