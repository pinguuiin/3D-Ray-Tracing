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
// # include <stdint.h>	// fixed-width data types (such as uint32_t, uint64_t)

void	handle_gnl_error_and_exit(t_info *info, int gnl_flag);
int		isspace_but_not_newline(int c);
void	parse_ambient_lighting(t_amb *amb, char *str, uint32_t line_num);
void	parse_camera(t_cam *cam, char *str, uint32_t line_num);
void	parse_light(t_light *light, char *str, uint32_t line_num);
void	parse_sphere(char *str, uint32_t line_num);
void	parse_plane(char *str, uint32_t line_num);
void	parse_cylinder(char *str, uint32_t line_num);
void	display_parsing_error(const char *msg, uint32_t line_num);
void	clean_up_parsing_memory(t_info *info);

#endif
