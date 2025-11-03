/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piyu <piyu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 15:59:24 by ykadosh           #+#    #+#             */
/*   Updated: 2025/11/03 10:10:08 by piyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minirt.h"


// WARN: debugging function to check validity of parsing, delete when ready.
static void	print_whole_execution_data(t_info *info);


t_info	*get_info(void)
{
	static t_info	info;

	return (&info);
}

int	free_exit(char *s)
{
	t_info	*info;

	info = get_info();
	free(info->light);
	free(info->obj);
	if (info->img)
		mlx_delete_image(info->mlx, info->img);
	if (info->mlx)
		mlx_terminate(info->mlx);
	if (s)
	{
		ft_putendl_fd("Error", 2);
		ft_putendl_fd(s, 2);
	}
	return (1);
}


static void	key_handler(mlx_key_data_t keydata, void *param)
{
	t_info	*info;

	info = (t_info *)param;
	if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(info->mlx);
	else if (keydata.key == MLX_KEY_LEFT || keydata.key == MLX_KEY_RIGHT
		|| keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_DOWN
		|| keydata.key == MLX_KEY_Q || keydata.key == MLX_KEY_Z)
		move_camera(keydata, info);
	else if (keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_S
		|| keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_D)
		rotate_camera(keydata, info);
}

void	initialize_mlx(t_info *info)
{
	info->mlx = NULL;
	info->img = NULL;
	mlx_set_setting(MLX_STRETCH_IMAGE, 1);
	info->mlx = mlx_init(WIDTH, HEIGHT, "miniRT", true);
	if (!info->mlx)
		exit(free_exit("Instance initialization failed"));

	info->img = mlx_new_image(info->mlx, WIDTH, HEIGHT);
	if (!info->img)
		exit(free_exit("Image buffer creation failed"));
	if (mlx_image_to_window(info->mlx, info->img, 0, 0) == -1)
		exit(free_exit("Pushing image to window failed"));
}


/*
* Return Values:
* 1: MLX function failures (WARN: not sure about this one!)
* 2: invalid input - unexpected argument or misconfigured .rt file
* 3: fatal system error during parsing, such as failures of open(), malloc(),
* 		read() and close() functions, or buffer for get_next_line_revised() is
*		predefined as empty.
* 0: If program runs smoothly
*/
int	main(int argc, char *argv[])
{
	t_info	*info;

	info = get_info();

	parse_argument(argc, argv);
	parse_scene(info, argv[1]);

	// WARN: only for debugging.
	print_whole_execution_data(info);
	preprocessor(info);

	initialize_mlx(info);
	mlx_key_hook(info->mlx, &key_handler, &info);
	mlx_loop_hook(info->mlx, renderer, &info);
	mlx_loop(info->mlx);


	free_exit(NULL); // this does not actually exit the program, no worries
	return (SUCCESS);
}

// WARN: debugging function to make sure parsing is done right.
#include "stdio.h"
static void	print_whole_execution_data(t_info *info)
{
	printf("AMBIENT LIGHT DATA:\n\t");
	printf("ambient's color values:\n\t\t");
	printf("info->amb.r:	<%.20f>\n\t\t", info->amb.r);
	printf("info->amb.g:	<%.20f>\n\t\t", info->amb.g);
	printf("info->amb.b:	<%.20f>\n\n\n\n", info->amb.b);


	printf("CAMERA DATA:\n\t");
	printf("camera's position coordinates:\n\t\t");
	printf("X coordinate:	<%.20f>\n\t\t", info->cam.pos.x);
	printf("Y coordinate:	<%.20f>\n\t\t", info->cam.pos.y);
	printf("Z coordinate:	<%.20f>\n\n\t", info->cam.pos.z);

	printf("camera's orientation vector (direction):\n\t\t");
	printf("X axis:	<%.20f>\n\t\t", info->cam.direction.x);
	printf("Y axis:	<%.20f>\n\t\t", info->cam.direction.y);
	printf("Z axis:	<%.20f>\n\n\t", info->cam.direction.z);

	printf("camera's FOV\n\t\t");
	printf("FOV:	<%.20f>\n\n\n\n", info->cam.fov);
	

	printf("LIGHT SOURCE DATA (ARRAY):\n\n\t");

	int	i;

	i = 0;
	while (i < info->n_light)
	{
		printf("LIGHT[%d] DATA:\n\t", i);
		printf("Light[%d]'s POSITION coordinates\n\t\t", i);

		printf("X coordinate:	<%.20f>\n\t\t", info->light[i].pos.x);
		printf("Y coordinate:	<%.20f>\n\t\t", info->light[i].pos.y);
		printf("Z coordinate:	<%.20f>\n\n\t", info->light[i].pos.z);

		printf("Light source's COLOR (RGB channels)\n\t\t");
		printf("R channel:	<%.20f>\n\t\t", info->light[i].color.r);
		printf("G channel:	<%.20f>\n\t\t", info->light[i].color.g);
		printf("B channel:	<%.20f>\n\n\n", info->light[i].color.b);

		i++;
		if (i < info->n_light)
			printf("\t");
	}
	printf("\n\n\n");

	// print objects array
	i = 0;
	printf("ALL SPHERES DATA:\n\n\t");
	while (i < info->n_obj && info->obj[i].type == SPHERE)
	{
		printf("SPHERE[%d]:\n\t", i);
		printf("Sphere[%d]'s POSITION coordinates:\n\t\t", i);
		printf("X coordinate:	<%.20f>\n\t\t", info->obj[i].pos.x);
		printf("Y coordinate:	<%.20f>\n\t\t", info->obj[i].pos.y);
		printf("Z coordinate:	<%.20f>\n\n\t", info->obj[i].pos.z);

		printf("Sphere[%d]'s RADIUS:\n\t\t", i);
		printf("radius is:	<%.20f>\n\n\t", info->obj[i].r);

		printf("Sphere[%d]'s COLOR (rgb channels):\n\t\t", i);
		printf("R channel:	<%.20f>\n\t\t", info->obj[i].color.r);
		printf("G channel:	<%.20f>\n\t\t", info->obj[i].color.g);
		printf("B channel:	<%.20f>\n\n\n", info->obj[i].color.b);

		i++;
		if (i < info->n_obj && info->obj[i].type == SPHERE)
			printf("\t");
	}
	printf("\n\n");

	printf("ALL PLANES DATA:\n\n\t");
	while (i < info->n_obj && info->obj[i].type == PLANE)
	{
		printf("PLANE[%d]:\n\t", i);
		printf("Plane[%d]'s POSITION coordinates:\n\t\t", i);
		printf("X coordinate:	<%.20f>\n\t\t", info->obj[i].pos.x);
		printf("Y coordinate:	<%.20f>\n\t\t", info->obj[i].pos.y);
		printf("Z coordinate:	<%.20f>\n\n\t", info->obj[i].pos.z);

		printf("Plane[%d]'s NORMAL VECTOR:\n\t\t", i);
		printf("X axis:	<%.20f>\n\t\t", info->obj[i].normal.x);
		printf("Y axis:	<%.20f>\n\t\t", info->obj[i].normal.y);
		printf("Z axis:	<%.20f>\n\n\t", info->obj[i].normal.z);

		printf("Plane[%d]'s COLOR (rgb channels):\n\t\t", i);
		printf("R channel:	<%.20f>\n\t\t", info->obj[i].color.r);
		printf("G channel:	<%.20f>\n\t\t", info->obj[i].color.g);
		printf("B channel:	<%.20f>\n\n\n", info->obj[i].color.b);

		i++;
		if (i < info->n_obj && info->obj[i].type == PLANE)
			printf("\t");
	}
	printf("\n\n");


	printf("ALL CYLINDERS DATA:\n\n\t");
	while (i < info->n_obj && info->obj[i].type == CYLINDER)
	{
		printf("CYLINDER[%d]:\n\t", i);
		printf("Cylinder[%d]'s POSITION coordinates:\n\t\t", i);
		printf("X coordinate:	<%.20f>\n\t\t", info->obj[i].pos.x);
		printf("Y coordinate:	<%.20f>\n\t\t", info->obj[i].pos.y);
		printf("Z coordinate:	<%.20f>\n\n\t", info->obj[i].pos.z);

		printf("Cylinder[%d]'s VECTOR OF CENTRAL AXIS:\n\t\t", i);
		printf("X axis:	<%.20f>\n\t\t", info->obj[i].normal.x);
		printf("Y axis:	<%.20f>\n\t\t", info->obj[i].normal.y);
		printf("Z axis:	<%.20f>\n\n\t", info->obj[i].normal.z);

		printf("Cylinder[%d]'s RADIUS:\n\t\t", i);
		printf("radius is:	<%.20f>\n\n\t", info->obj[i].r);

		printf("Cylinder[%d]'s HEIGHT:\n\t\t", i);
		printf("height is:	<%.20f>\n\n\t", info->obj[i].h);

		printf("Cylinder[%d]'s COLOR (rgb channels):\n\t\t", i);
		printf("R channel:	<%.20f>\n\t\t", info->obj[i].color.r);
		printf("G channel:	<%.20f>\n\t\t", info->obj[i].color.g);
		printf("B channel:	<%.20f>\n\n\n", info->obj[i].color.b);

		i++;
		if (i < info->n_obj && info->obj[i].type == CYLINDER)
			printf("\t");
	}
	printf("\n\n");
}
