/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 23:34:48 by piyu              #+#    #+#             */
/*   Updated: 2025/09/26 03:46:11 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	clamp(int single_channel_color)
{
	if (single_channel_color > 255)
		return (255);
	else if (single_channel_color < 0)
		return (0);
	else
		return (single_channel_color);
}

uint32_t	vec_to_color(t_vec color)
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;

	r = clamp(round(color.x * 255));
	g = clamp(round(color.y * 255));
	b = clamp(round(color.z * 255));
	return (r << 24 | g << 16 | b << 8 | 255);
}

t_vec	reflection(t_info *info, t_sphere *sp, t_vec ray, t_quad_coef f)
{
	double	k;
	t_hit	hit;

	k = (- f.b - sqrt(f.delta)) / (2 * f.a);
	// inside or on the object is painted to black for now
	if (k <= 0)
		return (vec3(0.0, 0.0, 0.0));
	ray = scale(ray, k);
	hit.pos = add(info->cam.pos, ray);
	hit.incoming = normalize(subtract(info->light.pos, hit.pos));
	hit.normal = normalize(subtract(hit.pos, sp->pos));
	hit.outgoing = subtract(scale(hit.normal, 2 * dot(hit.incoming, hit.normal)), hit.incoming);
	hit.ray = normalize(scale(ray, -1));
	hit.diffuse = scale(dot_elem(info->light.color, sp->color), info->light.ratio * KD * dot(hit.incoming, hit.normal));
	hit.specular = scale(info->light.color, info->light.ratio * KS * pow(dot(hit.outgoing, hit.ray), SHININESS));
	hit.intensity = add(hit.diffuse, hit.specular);
	return (hit.intensity);
}

void	draw_pixel(t_info *info, int x, int y)
{
	t_sphere	*sphere;
	t_vec		viewport;
	t_vec		ray;
	t_quad_coef	f;
	t_vec		color;

	sphere = (t_sphere *)info->obj;
	viewport = vec3(x * info->px - info->viewport_width / 2.0,
		-(y * info->px - info->viewport_height / 2.0), 0);
	rotate(&viewport, info->viewport_rot);
	ray = add(info->cam.direction, viewport);
	f.a = dot(ray, ray);
	f.b = 2 * dot(ray, sphere->oc);
	f.c = dot(sphere->oc, sphere->oc) - sphere->r * sphere->r;
	f.delta = f.b * f.b - 4.0 * f.a * f.c;
	color = scale(info->amb.color, info->amb.ratio);
	if (f.delta >= 0)
		color = add(color, reflection(info, sphere, ray, f)); //now ambient also inside object
	mlx_put_pixel(info->img, x, y, vec_to_color(color));
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
