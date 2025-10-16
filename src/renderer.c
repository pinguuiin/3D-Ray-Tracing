/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 23:34:48 by piyu              #+#    #+#             */
/*   Updated: 2025/10/17 00:39:02 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/* Implement Phong reflection model:
Diffuse = Kd (incoming light · object normal)
=> Diffuse term is counted when its dot product is greater than 0
Specular = Ks (Reflected ray · ray to camera) ^ Shininess
=> Specular term is counted when both terms' dot products greater than 0;
Intensity = Diffuse + Specular (+ Ambient), and clamped to 0-255
*/
t_vec	reflection(t_info *info, t_vec ray, t_hit *hit, double k)
{
	t_object	*obj;
	double		dot1;
	double		dot2;
	double		hit_h;

	obj = &info->obj[hit->obj_id];
	ray = scale(ray, k);
	hit->pos = add(info->cam.pos, ray);
	hit->ray = normalize(scale(ray, -1));
	hit->op = subtract(hit->pos, obj->pos);
	hit->incoming = normalize(subtract(info->light.pos, hit->pos));
	if (obj->type == SPHERE)
		hit->normal = normalize(hit->op);
	else if (obj->type == CYLINDER)
	{
		hit_h = dot(hit->op, obj->normal);
		if (hit_h - obj->h > -EPSILON)
			hit->normal = obj->normal;
		else if (hit_h + obj->h < EPSILON)
			hit->normal = scale(obj->normal, -1);
		else
			hit->normal = normalize(subtract(hit->op, scale(obj->normal, hit_h)));
	}
	else if (obj->type == PLANE)
		hit->normal = obj->normal;
	hit->outgoing = subtract(scale(hit->normal, 2 * dot(hit->incoming, hit->normal)), hit->incoming);
	hit->intensity = vec3(0.0, 0.0, 0.0);
	dot1 = dot(hit->incoming, hit->normal);
	if (dot1 > EPSILON)
	{
		hit->diffuse = scale(dot_elem(info->light.color, obj->color), info->light.ratio * KD * dot1);
		hit->intensity = add(hit->intensity, hit->diffuse);
		dot2 = dot(hit->outgoing, hit->ray);
		if (dot2 > EPSILON)
		{
			hit->specular = scale(info->light.color, info->light.ratio * KS * pow(dot2, SHININESS));
			hit->intensity = add(hit->intensity, hit->specular);
		}
	}
	return (hit->intensity);
}

double	ray_hit_sphere(t_info *info, t_vec ray, int id)
{
	t_object	*sphere;
	t_discrim	f;

	sphere = &info->obj[id];
	f.a = dot(ray, ray);
	f.b = 2 * dot(ray, sphere->oc);
	f.c = dot(sphere->oc, sphere->oc) - sphere->r * sphere->r;
	f.delta = f.b * f.b - 4.0 * f.a * f.c;
	if (f.delta >= EPSILON) // delta = 0, ray is tangent to the sphere, hit; root = 0, camera on the sphere, ray hit
	{
		f.root = (- f.b - sqrt(f.delta)) / (2 * f.a);
		if (f.root >= EPSILON)
			return (f.root);
		// else inside the sphere or sphere behind camera
	}
	return (-1.0);
}

/*
root = -(f.a / f.b)
f.a = 0, camera on the plane, ray hit, k=0;
f.a != 0, f.b = 0, ray parallel to the plane, not hit
f.a != 0, f.b != 0, root > 0, ray hit
*/
double	ray_hit_plane(t_info *info, t_vec ray, int id)
{
	t_object	*plane;
	t_discrim	f;

	plane = &info->obj[id];
	f.a = dot(plane->oc, plane->normal);
	f.b = dot(ray, plane->normal);
	if (fabs(f.a) < EPSILON)
		return (0.0);
	if (fabs(f.b) > EPSILON)
	{
		f.root = -(f.a / f.b);
		if (f.root > EPSILON)
			return (f.root);
	}
	return (-1.0);
}

double	hit_side_or_cap(t_info *info, t_vec ray, int id, t_discrim f)
{
	t_object	*cy;
	t_vec		cap_pos;
	t_vec		cap_oc;
	double		hit_h[2];

	cy = &info->obj[id];
	hit_h[0] = dot(add(cy->oc, scale(ray, f.root)), cy->normal);
	if (fabs(hit_h[0]) - cy->h > EPSILON)  // closer intersection point P is out of boundary
	{
		f.root2 = (- f.b + sqrt(f.delta)) / (2 * f.a);
		hit_h[1] = dot(add(cy->oc, scale(ray, f.root2)), cy->normal);
		if (hit_h[0] * hit_h[1] > EPSILON && fabs(hit_h[1]) - cy->h > EPSILON)  // both intersection points are out of boundaries
			return (-1.0);
		if (hit_h[0] > 0)
			cap_pos = add(cy->pos, scale(cy->normal, cy->h));
		else
			cap_pos = subtract(cy->pos, scale(cy->normal, cy->h));
		cap_oc = subtract(info->cam.pos, cap_pos);
		f.a = dot(cap_oc, cy->normal);
		f.b = dot(ray, cy->normal);
		f.root = -(f.a / f.b);
		return (f.root);
	}
	return (f.root);
}

double	hit_cap_or_inside(t_info *info, t_vec ray, int id, t_discrim f)
{
	t_object	*cy;
	t_vec		cap_pos;
	t_vec		cap_oc;
	double		hit_h[2];

	cy = &info->obj[id];
	hit_h[0] = dot(add(cy->oc, scale(ray, f.root2)), cy->normal);
	if (fabs(hit_h[0]) - cy->h > EPSILON)  // intersection point P is out of boundary
		return (-1.0);
	// {
	// 	f.root2 = (- f.b + sqrt(f.delta)) / (2 * f.a);
	// 	hit_h[1] = dot(add(cy->oc, scale(ray, f.root2)), cy->normal);
	// 	if (hit_h[0] * hit_h[1] > EPSILON && fabs(hit_h[1]) - cy->h > EPSILON)  // both intersection points are out of boundaries
	// 		return (-1.0);
		if (hit_h[0] > 0)
			cap_pos = add(cy->pos, scale(cy->normal, cy->h));
		else
			cap_pos = subtract(cy->pos, scale(cy->normal, cy->h));
		cap_oc = subtract(info->cam.pos, cap_pos);
		f.a = dot(cap_oc, cy->normal);
		f.b = dot(ray, cy->normal);
		f.root = -(f.a / f.b);
		return (f.root);
	}
	return (f.root);
}

double	ray_hit_cylinder(t_info *info, t_vec ray, int id)
{
	t_object	*cy;
	t_discrim	f;
	double		dot_oc_n;
	double		dot_ray_n;

	cy = &info->obj[id];
	dot_oc_n = dot(cy->oc, cy->normal);
	dot_ray_n = dot(ray, cy->normal);
	f.a = dot(ray, ray) - dot_ray_n * dot_ray_n;
	f.b = 2 * (dot(cy->oc, ray) - dot_oc_n * dot_ray_n);
	f.c = dot(cy->oc, cy->oc) - dot_oc_n * dot_oc_n - cy->r * cy->r;
	if (fabs(f.a) < EPSILON)  // ray on the axis of the cylinder, ray hit
	{
		if (dot(ray, cy->oc) < -EPSILON)  //object center in front of cam; inside cylinder is partially here!
			return (norm(cy->oc) - cy->h);
		else  //object center behind cam; inside cylinder is also partially here!
			return (-1.0);
	}
	f.delta = f.b * f.b - 4.0 * f.a * f.c;
	if (f.delta >= EPSILON)  // delta = 0, ray is tangent to the cylinder, hit; root = 0, camera on the cylinder, ray hit
	{
		f.root = (- f.b - sqrt(f.delta)) / (2 * f.a);
		if (f.root >= EPSILON)  // ray from outside the infinitely long cylinder
			return (hit_side_or_cap(info, ray, id, f));
		f.root2 = (- f.b + sqrt(f.delta)) / (2 * f.a);
		if (f.root2 >= EPSILON)  // ray from inside the infinitely long cylinder
			return (hit_cap_or_inside(info, ray, id, f));
		// else inside the cylinder or cylinder behind camera
	}
	return (-1.0);
}

double	nearest_ray_hit(t_info *info, t_vec ray, t_hit *hit)
{
	double	k;
	double	k_min;
	int		id;

	k = -1.0;
	k_min = -1.0;
	id = 0;
	while (id < info->num)
	{
		if (info->obj[id].type == SPHERE)
			k = ray_hit_sphere(info, ray, id);
		else if (info->obj[id].type == PLANE)
			k = ray_hit_plane(info, ray, id);
		else
			k = ray_hit_cylinder(info, ray, id);
		if (k >= 0.0)
		{
			if (k_min < -EPSILON || k < k_min)
			{
				k_min = k;
				hit->obj_id = id;
			}
		}
		id++;
	}
	return (k_min);
}

void	draw_pixel(t_info *info, t_vec ray, int x, int y)
{
	double	k;
	t_vec	color;
	t_hit	hit;

	k = nearest_ray_hit(info, ray, &hit);
	if (k == -1) // not hit
	{
		mlx_put_pixel(info->img, x, y, vec_to_color(vec3(0.0, 0.0, 0.0)));
		return ;
	}
	color = scale(info->amb.color, info->amb.ratio);
	color = add(color, reflection(info, ray, &hit, k));  // when camera on the object, k=0, the return will only include diffuse
	mlx_put_pixel(info->img, x, y, vec_to_color(color));
}

void	renderer(void *param)
{
	t_info		*info;
	int			x;
	int			y;
	t_vec		ray;


	info = (t_info *)param;
	x = 0;
	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
			ray = vec3(x * info->px - info->viewport_width / 2.0,
			-(y * info->px - info->viewport_height / 2.0), 0);
			rotate(info->rot, &ray);
			ray = add(info->cam.direction, ray);
			draw_pixel(info, ray, x, y);
			y++;
		}
		x++;
	}
}
