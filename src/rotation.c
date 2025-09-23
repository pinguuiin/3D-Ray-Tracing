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
	if (sq_len1 == 0 || sq_len2 == 0)
		return (0.0);
	return (acos((x1 * x2 + y1 * y2) / (sqrt(sq_len1) * sqrt(sq_len2))));
}

void	get_viewport_rotation(t_info *info, t_vec v1, t_vec v2)
{
	info->viewport_rot[0] = get_angle_between_vectors(v1.y, v1.z, v2.y, v2.z);
	info->viewport_rot[1] = get_angle_between_vectors(v1.x, v1.z, v2.x, v2.z);
	info->viewport_rot[2] = get_angle_between_vectors(v1.x, v1.y, v2.x, v2.y);
	if (cross(vec3(0, v1.y, v1.z), vec3(0, v2.y, v2.z)).x > 0)
		info->viewport_rot[0] *= -1;
	if (cross(vec3(v1.x, 0, v1.z), vec3(v2.x, 0, v2.z)).y > 0)
		info->viewport_rot[1] *= -1;
	if (cross(vec3(v1.x, v1.y, 0), vec3(v2.x, v2.y, 0)).z > 0)
		info->viewport_rot[2] *= -1;
	// #include <stdio.h>
	// printf("theta are: %f\t%f\t%f\n", theta[0], theta[1], theta[2]);
	// printf("v1: %f %f, v2: %f %f\n", v1.x, v1.y, v2.x, v2.y);
}

void	rotate(t_vec *vec, double *theta)
{
	rotate_x(vec, theta[0]);
	rotate_y(vec, theta[1]);
	rotate_z(vec, theta[2]);
}
