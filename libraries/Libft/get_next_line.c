/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 18:56:12 by lzhang2           #+#    #+#             */
/*   Updated: 2024/10/08 14:01:14 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_ptr(char **ptr)
{
	free(*ptr);
	*ptr = NULL;
}

static char	*get_line(char **backup, char **line)
{
	char	*next_backup;
	int		i;

	i = 0;
	next_backup = NULL;
	while (*(*backup + i) != '\n' && *(*backup + i) != '\0')
		i++;
	if (*(*backup + i) == '\n')
	{
		i++;
		*line = ft_substr(*backup, 0, i);
		next_backup = ft_strdup(*backup + i);
	}
	else
		*line = ft_strdup(*backup);
	ft_free_ptr(backup);
	return (next_backup);
}

static int	read_line(int fd, char **buffer, char **backup, char **line)
{
	int		bytes_read;
	char	*temporary;

	bytes_read = 1;
	while (!ft_strchr(*backup, '\n') && bytes_read)
	{
		bytes_read = read(fd, *buffer, BUFFER_SIZE);
		(*buffer)[bytes_read] = '\0';
		temporary = *backup;
		*backup = ft_strjoin(temporary, *buffer);
		free(temporary);
	}
	ft_free_ptr(buffer);
	*backup = get_line(backup, line);
	if (!(**line))
		ft_free_ptr(line);
	return (bytes_read);
}

char	*get_next_line(int fd)
{
	static char	*buffer_backup = NULL;
	char		*buffer;
	char		*line;

	if (fd < 0 || fd >= 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	if (read(fd, buffer, 0) < 0)
	{
		free(buffer);
		return (NULL);
	}
	if (!buffer_backup)
		buffer_backup = ft_strdup("");
	if (!read_line(fd, &buffer, &buffer_backup, &line) && !line)
		return (NULL);
	return (line);
}
// char	*extract_line(char *stash)
// {
// 	char	*line;
// 	int		i;
// 	int		len;

// 	if (stash == NULL)
// 		return (NULL);
// 	len = 0;
// 	while (stash[len] != '\n' && stash[len] != '\0')
// 		len++;
// 	if ((stash)[len] == '\n')
// 		len++;
// 	line = malloc((len + 1) * sizeof(char));
// 	if (!line)
// 		return (NULL);
// 	i = 0;
// 	while (i < len)
// 	{
// 		line[i] = stash[i];
// 		i++;
// 	}
// 	line[i] = '\0';
// 	return (line);
// }

// char	*read_and_line(int fd, char **stash, char *buf)
// {
// 	ssize_t	byte_read;
// 	char	*line;

// 	if (*stash == NULL)
// 		*stash = ft_strdup("");
// 	while (!ft_strchr(*stash, '\n'))
// 	{
// 		byte_read = read(fd, buf, BUFFER_SIZE);
// 		if (byte_read <= 0)
// 		{
// 			if (**stash && byte_read == 0)
// 			{
// 				buf[byte_read] = '\0';
// 				break ;
// 			}
// 			return (free (*stash), *stash = NULL, NULL);
// 		}
// 		buf[byte_read] = '\0';
// 		*stash = ft_strjoin(*stash, buf);
// 		if (!*stash)
// 			return (free (*stash), *stash = NULL, NULL);
// 	}
// 	line = extract_line(*stash);
// 	if (!line)
// 		return (free(*stash), *stash = NULL, NULL);
// 	return (line);
// }
// /*if (**stash != '\0' && byte_read == 0)   break; 
// 如果 stash 中还有数据，那么就跳出循环（假设这段代码在循环中），继续处理 stash 中的数据*/

// char	*clean_stash(char *stash)
// {
// 	size_t	len;
// 	char	*cleaned_stash;

// 	if (stash == NULL)
// 		return (NULL);
// 	len = 0;
// 	while (stash[len] != '\n' && stash[len] != '\0')
// 		len++;
// 	if (stash[len] == '\n')
// 		len++;
// 	cleaned_stash = ft_strdup(stash + len);
// 	free(stash);
// 	return (cleaned_stash);
// }
// /*stash 是一个指向字符数组的指针，stash + len 表示将指针向后移动 len 个字符的位置。
// 这样做可以得到从 stash 的第 len 个字符开始的子串的起始地址。*/

// char	*get_next_line(int fd)
// {
// 	static char	*stash;
// 	char		*buf;
// 	char		*line;

// 	if (fd < 0 || BUFFER_SIZE <= 0)
// 		return (NULL);
// 	buf = malloc((BUFFER_SIZE + 1) * sizeof(char));
// 	if (!buf)
// 		return (NULL);
// 	line = read_and_line(fd, &stash, buf);
// 	free (buf);
// 	if (!line)
// 	{
// 		if (stash)
// 		{
// 			free (stash);
// 			stash = NULL;
// 			return (NULL);
// 		}
// 		return (NULL);
// 	}
// 	stash = clean_stash(stash);
// 	return (line);
// }
