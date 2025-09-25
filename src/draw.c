/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 23:34:48 by piyu              #+#    #+#             */
/*   Updated: 2025/09/26 01:00:00 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

uint8_t	clamp(uint8_t single_channel_color)
{
	if (single_channel_color > 255)
		return (255);
	else if (single_channel_color < 0)
		return (0);
	else
		return (single_channel_color);
}

uint32_t	scale_color(uint32_t color, double ratio)
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;

	r = (color >> 24 & 0xFF) * ratio;
	g = (color >> 16 & 0xFF) * ratio;
	b = (color >> 8 & 0xFF) * ratio;
	return (r << 24 | g << 16 | b << 8 | 255);
}

uint32_t	reflect_ray(t_info *info, t_sphere *sp, t_vec ray, t_quad_coef f)
{
	double	k;
	t_hit	hit;

	k = (- f.b - sqrt(f.delta)) / (2 * f.a);
	// inside or on the object is painted to black for now
	if (k <= 0)
		return (0x000000FF);
	ray = scale(ray, k);
	hit.pos = add(info->cam.pos, ray);
	hit.incidence = normalize(subtract(info->light.pos, hit.pos));
	hit.normal = normalize(subtract(hit.pos, sp->pos));
	hit.specular = subtract(scale(hit.normal, 2 * dot(hit.incidence, hit.normal)), hit.incidence);
	hit.ray = normalize(scale(ray, -1));
	hit.intensity = add(scale(dot_elem(info->light.color, sp->color), KD * dot(hit.incidence, hit.normal)),
			scale(vec3(1.0, 1.0, 1.0), KS * pow(dot(hit.specular, hit.ray), SHININESS)));
	r = 255;
	g = 255;
	b = 255;
	return (r << 24 | g << 16 | b << 8 | 255);
}

void	draw_pixel(t_info *info, int x, int y)
{
	t_sphere	*sphere;
	t_vec		viewport;
	t_vec		ray;
	t_quad_coef	f;
	uint32_t	color;

	sphere = (t_sphere *)info->obj;
	viewport = vec3(x * info->px - info->viewport_width / 2.0,
		-(y * info->px - info->viewport_height / 2.0), 0);
	rotate(&viewport, info->viewport_rot);
	ray = add(info->cam.direction, viewport);
	f.a = dot(ray, ray);
	f.b = 2 * dot(ray, sphere->oc);
	f.c = dot(sphere->oc, sphere->oc) - sphere->r * sphere->r;
	f.delta = f.b * f.b - 4.0 * f.a * f.c;
	color = scale_color(info->amb.color, info->amb.ratio);
	if (f.delta >= 0)
		color += reflect_ray(info, sphere, ray, f);
	mlx_put_pixel(info->img, x, y, color);
}

void	draw(void *param)
{
	t_info		*info;
	int			x;
	int			y;

	info = (t_info *)param;
	x = 0;
	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
			draw_pixel(info, x, y);
			y++;
		}
		x++;
	}
}
