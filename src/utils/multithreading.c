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

#include <pthread.h>

// FIXME: does this function have to be called from within free_exit()?
// Be mindful of that as the code develops.
void	create_threads(t_info *info)
{
	size_t	i;

	i = 0;
	while (i < 5)
	{
		if (pthread_create(&info->threads[i], NULL, &render_routine, info)
		{
			destroy_threads(info, i);
			exit (free_exit("Failed to create a thread; Aborting miniRT", SYSTEM_FAILURE);
		}
		i++;
	}
}

// FIXME: 
// - how do I organize the threads' routine, so that they know when a frame has ended? use a flag?
// - how is the renderer() going to quit if something fails / if ESC or the x button has been pressed by the user?
// - how does each thread know which thread they are from the array?
void	render_routine(void *ptr)
{
	t_info	*info;

	info = (t_info *)ptr;



void	destroy_threads(t_info *info, size_t i)
{
	size_t	j;

	j = 0;
	while (j < i)
	{
		pthread_join(info->threads[i], NULL);
		j++;
	}
}






