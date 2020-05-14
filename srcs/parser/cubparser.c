/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cubparser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvalette <bvalette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 10:32:56 by bvalette          #+#    #+#             */
/*   Updated: 2020/05/02 15:22:37 by bvalette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"
#include <fcntl.h>

static void		ft_colors_parser(char *str, t_data *data)
{
	t_RGB_int	col;
	char		type;

	type = *str;
	str++;
	while (*str != '\0' && *str == ' ')
		str++;
	col.r = ft_atoi(str);
	while (*str != '\0' && (ft_isdigit(*str) == TRUE || *str == ' '))
		str++;
	str++;
	col.g = ft_atoi(str);
	while (*str != '\0' && (ft_isdigit(*str) == TRUE || *str == ' '))
		str++;
	str++;
	col.b = ft_atoi(str);
	while(ft_isdigit(*str) == TRUE || *str == ' ')
		str++;
	if (*str != '\0')
		return ;
	if (type == 'F' && col.r < 256 && col.g < 256 && col.b < 256)
		data->colors->f_color = ft_col_conv(data, col.r, col.g, col.b, 0);
	else if (type == 'C' && col.r < 256 && col.g < 256 && col.b < 256)
		data->colors->c_color = ft_col_conv(data, col.r, col.g, col.b, 0);
}

static void		ft_resolution_parser(char *str, t_data *data)
{
	while (*str != '\0' && ft_isalpha(*str) == TRUE)
		str++;
	data->res->x = ft_atoi(str);
	while (*str != '\0' && *str == ' ')
		str++;
	while (*str != '\0' && ft_isdigit(*str) == TRUE)
		str++;
	data->res->y = ft_atoi(str);
}

static int		ft_dispatch_parser(char *str, t_data **data)
{
	int		ret;

	ret = TRUE;
	if (ft_strncmp(str, "NO ", 3) == 0)
		ret = ft_textures_parser(str, *data);
	else if (ft_strncmp(str, "SO ", 3) == 0)
		ret = ft_textures_parser(str, *data);
	else if (ft_strncmp(str, "WE ", 3) == 0)
		ret = ft_textures_parser(str, *data);
	else if (ft_strncmp(str, "EA ", 3) == 0)
		ret = ft_textures_parser(str, *data);
	else if (ft_strncmp(str, "S ", 2) == 0)
		ret = ft_textures_parser(str, *data);
	else if (ft_strncmp(str, "R ", 2) == 0)
		ft_resolution_parser(str, *data);
	else if (ft_strncmp(str, "F ", 2) == 0)
		ft_colors_parser(str, *data);
	else if (ft_strncmp(str, "C ", 2) == 0)
		ft_colors_parser(str, *data);
	if (ret == TRUE)
		return (ret);
	else
		return (ft_free_all(*data, ret));
}

int				ft_parser(t_data *data)
{
	int		ret;
	int		ret_gnl;
	int		fd;
	char	*line;
	
	ret = TRUE;
	fd = open(data->files->cub_path, O_RDONLY);
	if (fd == ERROR)
		return (ft_free_all(data, ERROR_FILE));
	ret_gnl = TRUE;
	while (ret_gnl == TRUE && ret == TRUE)
	{
		ret_gnl = get_next_line(fd, &line);
		if (ret != ERROR && ft_is_mapdata(line) == TRUE)
			ft_map_parser(data, fd);
		else if (ret != ERROR && line[0] != ' ')
			ret = ft_dispatch_parser(line, &data);
		free(line);
	}
	if (ret <= ERROR)
		return (ft_free_all(data, ret));
	ret = ft_check_data(data);
	if (ret <= ERROR)
		return (ft_free_all(data, ret));
	ret = ft_map_checker(data);
	if (ret <= ERROR || data->map->set == FALSE)
		return (ft_free_all(data, ret));
	close(fd);
	return (TRUE);
}
