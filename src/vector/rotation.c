/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 23:35:05 by piyu              #+#    #+#             */
/*   Updated: 2025/11/29 20:57:50 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

inline void	rotate_x(t_vec *vec, double theta)
{
	double	temp;

	temp = vec->y;
	vec->y = vec->y * cos(theta) - vec->z * sin(theta);
	vec->z = vec->z * cos(theta) + temp * sin(theta);
}

inline void	rotate_y(t_vec *vec, double theta)
{
	double	temp;

	temp = vec->x;
	vec->x = vec->x * cos(theta) + vec->z * sin(theta);
	vec->z = vec->z * cos(theta) - temp * sin(theta);
}

inline void	rotate_z(t_vec *vec, double theta)
{
	double	temp;

	temp = vec->x;
	vec->x = vec->x * cos(theta) - vec->y * sin(theta);
	vec->y = vec->y * cos(theta) + temp * sin(theta);
}

// inline void	get_object_rot_matrix(double (*rot)[3], t_vec f)


/* For left-handed coordinate system (forward is +Z), Rot = [r u f]
forward (f) is the new camera direction, right (r) is the unit vector of f x up
new up (u) is the unit vector of r x f */
inline void	get_rotation_matrix(double (*rot)[3], t_vec f)
{
	t_vec	up;
	t_vec	r;
	t_vec	u;

	up = vec3(0, 1, 0);
	// Avoid zero vector when f and up are on the same or opposite directions
	if (fabs(dot(f, up)) > 1 - EPSILON)
		up = vec3(0, 0, -1);
	r = normalize(cross(f, up));
	u = cross(r, f);
	// (*rot)[3][3] = {
	// 	{r.x, u.x, f.x},
	// 	{r.y, u.y, f.y},
	// 	{r.z, u.z, f.z}
	// };
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
