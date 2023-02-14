/*
 * First attempt to write the function get_next_line()
 * This function will take fd as parameter and return a line until it finds '\n' or EOF ('\0') and return the current line (e.g. line 1).
 * It will remeber what is read last time, so when calling the function again, it will return the next line (e.g. line 2)
 * When nothing to be read or an error occurs when read(), return NULL
 *
 * problems about this code:
 * Didn't think about when to malloc and free the pointer throughroundly => have hardtime debugging and walk around the bugs.
 * The pseudocode is a mess and hard to read. Trying to make it simplier and hope to write better code.
 * */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

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

char	*ft_strjoin(char const *s1, char const *s2)
{
	char			*res;
	unsigned int	i;
	unsigned int	l;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	l = (unsigned int)ft_strlen(s1) + (unsigned int)ft_strlen(s2);
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
	free((char *)s1);
	return (res);
}

char	*ft_get_line(char *tmp)
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

char	*ft_left(char *tmp)
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

char	*ft_read_and_save(int fd, char *tmp)
{
	char	*buff;
	int		read_bytes;

	buff = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buff)
		return (NULL);
	if (!tmp)
	{
		tmp = malloc(1);
		if (!tmp)
			return (NULL);
		tmp[0] = '\0';
	}
	read_bytes = 1;
	while (!ft_strchr(tmp, '\n') && read_bytes != 0)
	{
		read_bytes = read(fd, buff, BUFFER_SIZE);
		if (read_bytes == -1)
		{
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

int    main(void)
{
    char *line;
    int    fd;

	line = "";
    fd = open("test.txt", O_RDONLY);

    if (fd == -1)
    {
        printf("fail to open the file.\n");
        return (1);
    }

    while (line != NULL)
    {
        line = get_next_line(fd);
        printf("%s", line);
        free(line);
    }
    printf("\n");
    close(fd);
    return(0);
}
