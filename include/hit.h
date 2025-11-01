/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 00:51:02 by piyu              #+#    #+#             */
/*   Updated: 2025/10/20 02:29:43 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HIT_H
# define HIT_H

# include "vector.h"

/* Coefficient a, b and c, discriminant (delta) and root of the function for hit detection */
typedef struct s_discrim
{
	double	a;
	double	b;
	double	c;
	double	delta;
	double	root;
	double	root2;
	double	oc_n;
	double	ray_n;

}	t_discrim;

/* Parameters used for the reflection model when ray intersects with the object */
typedef struct s_hit
{
	uint32_t	obj_id;  // hit object id
	t_vec		pos;  // coordinates of the hit point
	t_vec		op;  // vector from object center to intersection point
	t_vec		incoming;  // normalized ray direction from hit point to light source
	t_vec		normal;  // normal at the hit point
	t_vec		ray;  // normalized ray direction from hit point to camera
	t_vec		outgoing; // normalized specular reflected ray direction
	t_vec		diffuse;  // diffuse reflection intensity
	t_vec		specular;  // specular reflection intensity
	t_vec		intensity;  // intensity = diffuse + specular

}	t_hit;

#endif
