/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alopez-g <alopez-g@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 23:24:14 by alopez-g          #+#    #+#             */
/*   Updated: 2023/06/01 00:28:25 by alopez-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "reader.h"

void	map_add_line(t_scene *scene, char *const line);

static void	parse_line(t_scene *scene, char *const line, t_error_list *err)
{
	size_t	i;

	i = skip_space(line);
	if (!ft_strncmp(line + i, "NO", 2))
		reader_texture(scene, line + i + 2, err, NORTH);
	else if (!ft_strncmp(line + i, "SO", 2))
		reader_texture(scene, line + i + 2, err, SOUTH);
	else if (!ft_strncmp(line + i, "WE", 2))
		reader_texture(scene, line + i + 2, err, WEST);
	else if (!ft_strncmp(line + i, "EA", 2))
		reader_texture(scene, line + i + 2, err, EAST);
	else if (!ft_strncmp(line + i, "F", 1))
		reader_room_color(scene, line + i + 1, err, FLOOR);
	else if (!ft_strncmp(line + i, "C", 1))
		reader_room_color(scene, line + i + 1, err, CIELLING);
	else if (is_space(*(line + i)) || *(line + i) == '1')
		reader_map(scene, line, err);
	else if (!((*(line + i) == '#') || (*(line + i) == '\0')))
		error_list_add(err, error_new(UNKNOWN_OBJECT, line));
}

t_map_error	reader(t_scene *scene, char *const path)
{
	int				fd;
	char			*line;
	t_error_list	err_list;
	char			ret;

	ret = OK;
	error_list_init(&err_list);
	fd = open(path, O_RDONLY);
	if (-1 == fd)
		ret = MAP_NOT_FOUND;
	line = get_next_line(fd);
	while (line)
	{
		parse_line(scene, line, &err_list);
		free(line);
		line = get_next_line(fd);
	}
	map_add_line(scene, " ");
	if (ft_lstsize(err_list.err) > 1)
		ret = KO;
	error_list_log(&err_list);
	error_list_delete(&err_list);
	close(fd);
	printf("Read map:\n");
	scene_log_map(scene);
	return (ret);
}