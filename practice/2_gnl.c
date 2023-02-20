#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

size_t	ft_strlen(const char *s)
{
	int	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	ch;

	ch = (unsigned char)c;
	i = 0;
	if (!s)
		return (NULL);
	while (s[i] != '\0')
	{
		if (s[i] == ch)
			return ((char *)(&s[i]));
		i++;
	}
	if (ch == '\0')
		return ((char *)&s[i]);
	return (NULL);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char			*res;
	size_t	i;
	size_t	l;

	i = 0;
	l = ft_strlen(s1) + ft_strlen(s2);
	res = (char *)malloc((l + 1) * sizeof(char));
	if (!res)
		return (NULL);
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	l = 0;
	while (s2[l])
	{
		res[i + l] = s2[l];
		l++;
	}
	res[i + l] = '\0';
	free(s1);
	return (res);
}

char	*ft_strcpy(char *dst, const char *src)
{
	size_t	i;
	size_t	src_len;

	i = 0;
	src_len = 0;
	while (src[src_len++]);
	while (src_len != 0)
	{
		dst[i] = src[i];
		i++;
		src_len--;
	}
	return (dst);
}

static char	*ft_get_line(char *tmp)
{
	unsigned int	i;
	char	*s;

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

/* Optimize the ft_left function by using the strchr function to find the position of the newline character in the string.
 * And then allocating memory only for the remaining characters.
 * Reduce the number of unnecessary function calls and memory allocations.*/
static char	*ft_left(char *tmp)
{
	char	*left;
	char	*newline;

	newline = ft_strchr(tmp, '\n');
	if (!newline)
	{
		free(tmp);
		return (NULL);
	}
	left = (char *)malloc(sizeof(char) * (ft_strlen(newline + 1) + 1));
	if (!left)
		return (NULL);
	ft_strcpy(left, newline + 1);
	free(tmp);
	return (left);
}

/* Passing the tmp string by reference in the ft_read_and_save function to avoid unnecessary string copying.
 * Move assigning new string to tmp to get_next_line function*/
static char	*ft_read_and_save(int fd, char *tmp)
{
	char	*buff;
	int		read_bytes;

	buff = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buff)
		return (NULL);
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
	static char	*tmp;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if(!tmp)
	{
		tmp = (char *)malloc(1);
		if (!tmp)
			return (NULL);
		tmp[0] = '\0';
	}
	tmp = ft_read_and_save(fd, tmp);
	if (!tmp)
	{
		free(tmp);
		return (NULL);
	}
	line = ft_get_line(tmp);
	tmp = ft_left(tmp);
	return (line);
}

int	main(void)
{
	char	*line = "";
	int	fd;

	fd = open("fd1.txt", O_RDONLY);

	if (fd == -1)
	{
		printf("fail to open the file.");
		return (1);
	}

	while (line)
	{
		line = get_next_line(fd);
		printf("%s", line);
		free(line);
	}
	printf("\n");
	close(fd);

	return(0);
}
