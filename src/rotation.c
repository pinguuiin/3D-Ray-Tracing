/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 23:35:05 by piyu              #+#    #+#             */
/*   Updated: 2025/10/02 00:08:50 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	rotate_x(t_vec *vec, double theta)
{
	double	temp;

	temp = vec->y;
	vec->y = vec->y * cos(theta) - vec->z * sin(theta);
	vec->z = vec->z * cos(theta) + temp * sin(theta);
}

void	rotate_y(t_vec *vec, double theta)
{
	double	temp;

	temp = vec->x;
	vec->x = vec->x * cos(theta) + vec->z * sin(theta);
	vec->z = vec->z * cos(theta) - temp * sin(theta);
}

void	rotate_z(t_vec *vec, double theta)
{
	double	temp;

	temp = vec->x;
	vec->x = vec->x * cos(theta) - vec->y * sin(theta);
	vec->y = vec->y * cos(theta) + temp * sin(theta);
}

double	get_angle_between_vectors(double x1, double y1, double x2, double y2)
{
	double	sq_len1;
	double	sq_len2;

	sq_len1 = x1 * x1 + y1 * y1;
	sq_len2 = x2 * x2 + y2 * y2;
	if (sq_len1 < EPSILON || sq_len2 < EPSILON)
		return (0.0);
	return (acos((x1 * x2 + y1 * y2) / (sqrt(sq_len1) * sqrt(sq_len2))));
}

// void	get_viewport_rotation(t_info *info, t_vec v1, t_vec v2)
// {
// 	info->viewport_rot[0] = get_angle_between_vectors(v1.y, v1.z, v2.y, v2.z);
// 	info->viewport_rot[1] = get_angle_between_vectors(v1.x, v1.z, v2.x, v2.z);
// 	info->viewport_rot[2] = get_angle_between_vectors(v1.x, v1.y, v2.x, v2.y);
// 	if (cross(vec3(0, v1.y, v1.z), vec3(0, v2.y, v2.z)).x > EPSILON)
// 		info->viewport_rot[0] *= -1;
// 	if (cross(vec3(v1.x, 0, v1.z), vec3(v2.x, 0, v2.z)).y > EPSILON)
// 		info->viewport_rot[1] *= -1;
// 	if (cross(vec3(v1.x, v1.y, 0), vec3(v2.x, v2.y, 0)).z > EPSILON)
// 		info->viewport_rot[2] *= -1;
// 	#include <stdio.h>
// 	printf("cam dir: %f\t%f\t%f\n", info->cam.direction.x, info->cam.direction.y, info->cam.direction.z);
// 	printf("theta are: %f\t%f\t%f\n", info->viewport_rot[0], info->viewport_rot[1], info->viewport_rot[2]);
// 	printf("cross: %f\n\n", cross(vec3(v1.x, 0, v1.z), vec3(v2.x, 0, v2.z)).y);
// }

/* For left-handed coordinate system (forward is +Z), Rot = [s u f]
forward (f) is the new camera direction, right (r) is the unit vector of f x up
new up (u) is the unit vector of r x f */
void	get_rotation_matrix(t_info *info, t_vec v)
{
	t_vec	up;
	t_vec	f;
	t_vec	r;
	t_vec	u;

	// #include <stdio.h>
	// printf("before rotate: %f\n", norm(v));
	// printf("ray dir: %f %f %f\n", vec->x, vec->y, vec->z);


	up = vec3(0, 1, 0);
	f = normalize(v);
	// Avoid zero vector when f and up are on the same or opposite direction
	if (fabs(dot(f, up)) > 1 - EPSILON)
		up = vec3(0, 0, -1);
	r = normalize(cross(f, up));
	u = cross(r, f);
	// info->rot[3][3] = {
	// 	{r.x, u.x, f.x},
	// 	{r.y, u.y, f.y},
	// 	{r.z, u.z, f.z}
	// };
	info->rot[0][0] = r.x;
	info->rot[1][0] = r.y;
	info->rot[2][0] = r.z;
	info->rot[0][1] = u.x;
	info->rot[1][1] = u.y;
	info->rot[2][1] = u.z;
	info->rot[0][2] = f.x;
	info->rot[1][2] = f.y;
	info->rot[2][2] = f.z;

	// rotate(info->rot, &v);
	// printf("after rotate: %f\n\n", norm(v));
}

void	rotate(double rot[3][3], t_vec *v1)
{
	t_vec	v2;

	v2.x = rot[0][0] * v1->x + rot[0][1] * v1->y + rot[0][2] * v1->z;
	v2.y = rot[1][0] * v1->x + rot[1][1] * v1->y + rot[1][2] * v1->z;
	v2.z = rot[2][0] * v1->x + rot[2][1] * v1->y + rot[2][2] * v1->z;
	v1->x = v2.x;
	v1->y = v2.y;
	v1->z = v2.z;
}
