/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 23:35:05 by piyu              #+#    #+#             */
/*   Updated: 2025/12/31 05:27:11 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

inline void	rotate_y(t_vec *vec, double theta)
{
	double	temp;

	temp = vec->x;
	vec->x = vec->x * cos(theta) + vec->z * sin(theta);
	vec->z = vec->z * cos(theta) - temp * sin(theta);
}

#ifndef BONUS
#else
/* Get rotation matrix [r u f] for an object. Define the forward vector as +Z,
upward vector as the rotation axis and right vector accordingly*/
inline void	get_object_rot_matrix(double (*rot)[3], t_vec u)
{
	t_vec	r;
	t_vec	f;

	f = vec3(0, 0, 1);
	if (fabs(dot(f, u)) > 1.0 - EPSILON)
		f = vec3(0, 1, 0);
	r = normalize(cross(f, u));
	f = cross(u, r);
	rot[0][0] = r.x;
	rot[0][1] = r.y;
	rot[0][2] = r.z;
	rot[1][0] = u.x;
	rot[1][1] = u.y;
	rot[1][2] = u.z;
	rot[2][0] = f.x;
	rot[2][1] = f.y;
	rot[2][2] = f.z;
}
#endif

/* For left-handed coordinate system (forward is +Z), Rot = [r u f]
forward (f) is the new camera direction, right (r) is the unit vector of f x up
new up (u) is the unit vector of r x f.
When f and up are on the same or opposite directions, move up to (0,0,-1)
to avoid cross product becoming zero vector */
inline void	get_rotation_matrix(double (*rot)[3], t_vec f, t_vec up)
{
	t_vec	r;
	t_vec	u;

	if (fabs(dot(f, up)) > 1.0 - EPSILON)
		up = vec3(0, 0, -1);
	r = normalize(cross(f, up));
	u = cross(r, f);
	rot[0][0] = r.x;
	rot[1][0] = r.y;
	rot[2][0] = r.z;
	rot[0][1] = u.x;
	rot[1][1] = u.y;
	rot[2][1] = u.z;
	rot[0][2] = f.x;
	rot[1][2] = f.y;
	rot[2][2] = f.z;
}

inline void	rotate(double rot[3][3], t_vec *v1)
{
	t_vec	v2;

	v2.x = rot[0][0] * v1->x + rot[0][1] * v1->y + rot[0][2] * v1->z;
	v2.y = rot[1][0] * v1->x + rot[1][1] * v1->y + rot[1][2] * v1->z;
	v2.z = rot[2][0] * v1->x + rot[2][1] * v1->y + rot[2][2] * v1->z;
	v1->x = v2.x;
	v1->y = v2.y;
	v1->z = v2.z;
}
