/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvalette <bvalette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 09:18:37 by bvalette          #+#    #+#             */
/*   Updated: 2020/05/17 20:11:36 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "cub3d.h"
#include "mlx.h"
#include <stdio.h>
#include <math.h>

static void		ft_img_init(t_data *data)
{
	t_window	*win;
	t_res		res;
	
	win = data->win;
	res.x = data->res->x;
	res.y = data->res->y;
	data->img[VIEW]->ptr = mlx_new_image(win->mlx_ptr, res.x, res.y);
	data->img[VIEW]->data = (int *)mlx_get_data_addr(data->img[VIEW]->ptr,
			&data->img[VIEW]->bpp, &data->img[VIEW]->size_line, &win->endian);
	data->img[VIEW]->bpp /= 8;
	data->img[VIEW]->size_line /= data->img[VIEW]->bpp;
}

static void		ft_struct_failure(t_data *data, int failure)
{
	if (failure > 0)
		free(data->img[SO]);
	if (failure > 1)
		free(data->img[NO]);
	if (failure > 2)
		free(data->img[EA]);
	if (failure > 3)
		free(data->img[WE]);
	if (failure > 4)
		free(data->img[SP]);
	free(data->img);
	ft_free_all(data, ERROR_MALLOC_IM);
}

static void		ft_img_struct_init(t_data *data)
{
	data->img[NO] = malloc(sizeof(t_img));
	if (data->img[NO] == NULL)
		ft_struct_failure(data, 0);
	data->img[SO] = malloc(sizeof(t_img));
	if (data->img[SO] == NULL)
		ft_struct_failure(data, 1);
	data->img[EA] = malloc(sizeof(t_img));
	if (data->img[EA] == NULL)
		ft_struct_failure(data, 2);
	data->img[WE] = malloc(sizeof(t_img));
	if (data->img[WE] == NULL)
		ft_struct_failure(data, 3);
	data->img[SP] = malloc(sizeof(t_img));
	if (data->img[SP] == NULL)
		ft_struct_failure(data, 4);
	data->img[VIEW] = malloc(sizeof(t_img));
	if (data->img[VIEW] == NULL)
		ft_struct_failure(data, 5);
}

static int		ft_mlx_init(t_data *data)
{
	t_res			res;
	t_window		*win;

	win = data->win;
	res.x = data->res->x;
	res.y = data->res->y;
	win->mlx_ptr = mlx_init();
	if (win->mlx_ptr == NULL)
		return (ERROR_MLX);
	win->win_ptr = mlx_new_window(win->mlx_ptr, res.x, res.y, data->files->cub_path);
	if (win->win_ptr == NULL)
		return (ERROR_MLX);
	mlx_do_key_autorepeaton(data->win->mlx_ptr);
	mlx_hook(win->win_ptr, 6, (1L<<0), ft_mouse_manager, data);
	mlx_hook(win->win_ptr, 2, (1L<<0), ft_key_hook, data);
	mlx_hook(win->win_ptr, 17, (1L<<2), ft_escape, data);
	return (TRUE);
}

static void	ft_compute_colors(t_data *data)
{
	t_RGB_int	col;

	ft_memcpy(&col, &data->colors->f_color_rgb, sizeof (int) * 3);
	data->colors->f_color = ft_col_conv(data, col.r, col.g, col.b, 0);
	ft_memcpy(&col, &data->colors->c_color_rgb, sizeof (int) * 3);
	data->colors->c_color = ft_col_conv(data, col.r, col.g, col.b, 0);
}

int		ft_cub3d(t_data *data)
{
	int			ret;
	
	ft_img_struct_init(data);
	ret = ft_mlx_init(data);
	if (ret != TRUE)
		return (ERROR_MLX);
	ft_img_init(data);
	if (data->export_flag == TRUE)
		mlx_destroy_window(data->win->mlx_ptr, data->win->win_ptr);
	ft_compute_colors(data);
	ret = ft_import_textures(data);
	if (ret != TRUE)
		return (ret);
	if (data->export_flag == TRUE)
	{
		ft_render_view(data);
		ret = ft_save_to_file(data);
		ft_free_all(data, TRUE);
	}
	else
	{
		mlx_string_put(data->win->mlx_ptr, data->win->win_ptr,
data->res->x / 2 - 70, data->res->y / 2, RED, "Press any key to start");
		mlx_put_image_to_window(data->win->mlx_ptr, data->win->win_ptr, 
			data->img[SP]->ptr,	data->res->x / 2 - 32, data->res->y / 2 + 32); 
		ret = mlx_loop(data->win->mlx_ptr);
	}
	return (ret);
}
