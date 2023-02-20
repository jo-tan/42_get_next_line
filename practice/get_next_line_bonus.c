/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jo-tan <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 07:59:35 by jo-tan            #+#    #+#             */
/*   Updated: 2023/02/19 08:04:59 by jo-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*ft_get_line(char *tmp)
{
	unsigned int	i;
	char			*s;

	i = 0;
	if (!tmp[i])
		return (NULL);
	while (tmp[i] && tmp[i] != '\n')
		i++;
	s = (char *)malloc(sizeof(char) * (i + 2));
	if (!s)
		return (NULL);
	i = 0;
	while (tmp[i] && tmp[i] != '\n')
	{
		s[i] = tmp[i];
		i++;
	}
	if (tmp[i] == '\n')
	{
		s[i] = tmp[i];
		i++;
	}
	s[i] = '\0';
	return (s);
}

static char	*ft_save(char *tmp)
{
	int		i;
	int		j;
	char	*s;

	i = 0;
	while (tmp[i] && tmp[i] != '\n')
		i++;
	if (!tmp[i])
	{
		free(tmp);
		return (NULL);
	}
	s = (char *)malloc(sizeof(char) * (ft_strlen(tmp) - i + 1));
	if (!s)
		return (NULL);
	i++;
	j = 0;
	while (tmp[i])
		s[j++] = tmp[i++];
	s[j] = '\0';
	free(tmp);
	return (s);
}

static char	*ft_new_string(char *s)
{
	if (!s)
	{
		s = malloc(1);
		if (!s)
			return (NULL);
		s[0] = '\0';
	}
	return (s);
}

static char	*ft_read_and_save(int fd, char *tmp)
{
	char	*buff;
	int		read_bytes;

	buff = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buff)
		return (NULL);
	if (!tmp)
		tmp = ft_new_string(tmp);
	read_bytes = 1;
	while (!ft_strchr(tmp, '\n') && read_bytes != 0)
	{
		read_bytes = read(fd, buff, BUFFER_SIZE);
		if (read_bytes == -1)
		{
			free(tmp);
			free(buff);
			return (NULL);
		}
		buff[read_bytes] = '\0';
		tmp = ft_strjoin(tmp, buff);
	}
	free(buff);
	return (tmp);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*tmp[1024];

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	tmp[fd] = ft_read_and_save(fd, tmp[fd]);
	if (!tmp[fd])
		return (NULL);
	line = ft_get_line(tmp[fd]);
	tmp[fd] = ft_save(tmp[fd]);
	return (line);
}
