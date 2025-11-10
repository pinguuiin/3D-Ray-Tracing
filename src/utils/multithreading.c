/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multithreading.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykadosh <ykadosh@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:16:47 by ykadosh           #+#    #+#             */
/*   Updated: 2025/11/09 17:16:48 by ykadosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// WARN: include these headers from the main header, and remove from here.
#include <pthread.h>
#include <stdatomic.h>

static void	destroy_threads(t_info *info, int i);
static void	render_routine(void *ptr);


void	init_threads(t_info *info)
{
	int	i;

	i = 0;
	while (i < N_THREADS)
	{
		if (pthread_create(&info->threads[i].painter, NULL, &render_routine, info)
		{
			destroy_threads(info, i);
			exit (free_exit("Failed to create a thread; Aborting miniRT", SYSTEM_FAILURE);
		}
		info->threads[i].p_info = info;
		info->threads[i].x = WIDTH / N_THREADS * i;
		if (N_THREADS && i < N_THREADS - 1) // first part is just to avoid overflow in the unlikely but theoretically possible case where N_THREADS would be defined to 1!
			info->threads[i].border_x = WIDTH / N_THREADS * (i + 1);
		else
			info->threads[i].border_x = WIDTH; // important, otherwise a pixel (or more) could be lost and not rendered, because of floating point truncations. We let the last thread do those until the very last one.
		i++;
	}
}


// FIXME: does this function have to be called from within free_exit()?
// Be mindful of that as the code develops.
// WARN: could the join_thread() call be combined into the main loop?
// since it is a cleaning function anyways....
static void	destroy_threads(t_info *info, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		// WARN: can pthread_join() fail? handle the error if yes.
		pthread_join(info->threads[j].painter, NULL);
		j++;
	}
}


// FIXME:
// - how do I organize the threads' routine, so that they know when a frame has ended? use a flag?
// - how is the renderer() going to quit if something fails / if ESC or the x button has been pressed by the user?
// - how does each thread know which thread they are from the array?
static void	render_routine(void *ptr)
{
	t_painter	*painter;
	t_info		*info;
	t_vec		*ray;
	int			temp;
	int			y;

	painter = (t_painter *)ptr;
	info = painter->p_info;
	temp = painter->x;


	// TODO: add another outer layer of a loop, allowing each painter to restart
	// from zero once it is time to render a new frame. But this requires an extra
	// flag from the main process (renderer()).
	// TODO: the painter will check that flag from time to time, in order to
	// be able to 'know' if it needs to abort!! I guess this flag should be
	// set by the key_hooks - ESC && `X` button of the MLX window....
	
	while (1) OR while (ESC is not pressd && 'x' button is not pressed) OR while (!exit_flag) ????

	while (!info->should_render)
		usleep(50);
	painter->is_done = 0;
	painter->x = temp;
	while (painter->x < painter->border_x)
	{
		y = 0;
		while (y < HEIGHT)
		{
			ray = vec3(painter->x * info->px - info->viewport_width / 2.0,
			-(y * info->px - info->viewport_height / 2.0), 0);
			rotate(info->rot, &ray);
			ray = normalize(add(info->cam.direction, ray));
			draw_pixel(info, ray, painter->x, y);
			y++;
		}
		painter->x++;
	}
	painter->is_done = 1;
	// TODO: outer layer of loop should go all the way here.
}







